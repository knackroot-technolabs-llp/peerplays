/*
 * Copyright (c) 2015 Cryptonomex, Inc., and contributors.
 *
 * The MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#pragma once
#include <graphene/chain/protocol/types.hpp>
#include <graphene/db/object.hpp>
#include <graphene/db/generic_index.hpp>
#include <graphene/chain/witness_scheduler.hpp>
#include <graphene/chain/witness_scheduler_rng.hpp>

namespace graphene { namespace chain {

class witness_schedule_object;

//#ifndef GRAPHENE_SHUFFLED_WITNESSES
typedef hash_ctr_rng<
   /* HashClass  = */ fc::sha256,
   /* SeedLength = */ GRAPHENE_RNG_SEED_LENGTH
   > witness_scheduler_rng;

typedef generic_witness_scheduler<
   /* WitnessID  = */ witness_id_type,
   /* RNG        = */ witness_scheduler_rng,
   /* CountType  = */ decltype( chain_parameters::maximum_witness_count ),
   /* OffsetType = */ uint32_t,
   /* debug      = */ true
   > witness_scheduler;

typedef generic_far_future_witness_scheduler<
   /* WitnessID  = */ witness_id_type,
   /* RNG        = */ witness_scheduler_rng,
   /* CountType  = */ decltype( chain_parameters::maximum_witness_count ),
   /* OffsetType = */ uint32_t,
   /* debug      = */ true
   > far_future_witness_scheduler;
//#endif

class witness_schedule_object : public graphene::db::abstract_object<witness_schedule_object>
{
   public:
      static const uint8_t space_id = implementation_ids;
      static const uint8_t type_id = impl_witness_schedule_object_type;

//#ifdef GRAPHENE_SHUFFLED_WITNESSES
      vector< witness_id_type > current_shuffled_witnesses;
//#else
      witness_scheduler scheduler;
      uint32_t last_scheduling_block;
      uint64_t slots_since_genesis = 0;
      fc::array< char, sizeof(secret_hash_type) > rng_seed;

      /**
       * Not necessary for consensus, but used for figuring out the participation rate.
       * The nth bit is 0 if the nth slot was unfilled, else it is 1.
       */
      fc::uint128 recent_slots_filled;
 //#endif

};

} }


//#ifdef GRAPHENE_SHUFFLED_WITNESSES
FC_REFLECT( graphene::chain::witness_scheduler,
            (_turns)
            (_tokens)
            (_min_token_count)
            (_ineligible_waiting_for_token)
            (_ineligible_no_turn)
            (_eligible)
            (_schedule)
            (_lame_duck)
            )

//#ifdef GRAPHENE_SHUFFLED_WITNESSES
FC_REFLECT_DERIVED(
   graphene::chain::witness_schedule_object,
   (graphene::db::object),
   (scheduler)
   (last_scheduling_block)
   (slots_since_genesis)
   (rng_seed)
   (recent_slots_filled)
   (current_shuffled_witnesses)
)
