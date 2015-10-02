/***************************************************************************
 *   Copyright (C) 2015 Andrey Timashov                                    *
 *                                                                         *
 *   This file is part of Tetrahedrosaur.                                  *
 *                                                                         *
 *   Tetrahedrosaur is free software: you can redistribute it and/or       *
 *   modify it under the terms of the GNU General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   Tetrahedrosaur is distributed in the hope that it will be useful,     *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU      *
 *   General Public License for more details.                              *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with Tetrahedrosaur. If not, see <http://www.gnu.org/licenses/> *
 ***************************************************************************/


#include <cassert>
#include <cstdlib>


#include "roommates.hpp"


// Current implementaion is based on pascal code from original paper
// "An Efficient Algorithm for the "Stable Roommates" Problem" by
// Robert W. Irving


namespace algo {


namespace roommates {


namespace {


typedef size_t _RankType;


struct _State
{
   explicit _State(Matrix<PersonType> & preference);

   inline PersonType bestPotentialPartnerFor(PersonType person) const
   {
      return preference[person][leftmost[person]];
   }
   inline PersonType worstPotentialPartnerFor(PersonType person) const
   {
      return preference[person][rightmost[person]];
   }

   const size_t n;
   const Matrix<PersonType> & preference;
   const Matrix<_RankType> ranking;
   std::vector<_RankType> leftmost;
   std::vector<_RankType> rightmost;
};


_State::_State(Matrix<PersonType> & preference)
   : n(preference.size()), preference(preference), ranking(n)
{
   Matrix<_RankType> & nonconstRanking =
      const_cast<Matrix<_RankType> &>(ranking);
   for (PersonType person = 0; person < n; ++person)
   {
      preference[person][n - 1] = person; // Sentinel;
      for (_RankType rank = 0; rank < n; ++rank)
      {
         nonconstRanking[person][preference[person][rank]] = rank;
      }
   }

   // Last elements with index n are sentinels for procedure find;
   leftmost.resize(n + 1, 0);
   rightmost.resize(n + 1, n - 1);
}


bool _phase1Reduce(_State & st)
{
   PersonType proposer = 0;
   PersonType nextChoice = 0;
   std::vector<bool> proposedTo(st.n, false);
   for (PersonType person = 0; person < st.n; ++person)
   {
      proposer = person;
      do
      {
         // Proposer proposes until someone agrees to hold him;
         nextChoice = st.bestPotentialPartnerFor(proposer);
         PersonType current = st.worstPotentialPartnerFor(nextChoice);
         while (st.ranking[nextChoice][current] <
            st.ranking[nextChoice][proposer])
         { // Proposal is rejected, go to next person in preference list;
            ++st.leftmost[proposer];
            nextChoice = st.bestPotentialPartnerFor(proposer);
            current = st.worstPotentialPartnerFor(nextChoice);
         } // Sentinels in preference matrix won't let st.leftmost overflow;

         // Make nextChoice to hold a proposer and reject current;
         st.rightmost[nextChoice] = st.ranking[nextChoice][proposer];

         // Now newly rejected current will send proposals;
         proposer = current;
      }
      while (proposedTo[nextChoice]);
      proposedTo[nextChoice] = true;
   }
   return (proposer == nextChoice); // Solution possible;
}


void _findFirstUnmatched(const _State & st, PersonType & firstUnmatched)
{
   // Find first person with more then one potential partner;
   while (st.leftmost[firstUnmatched] == st.rightmost[firstUnmatched])
   {
      ++firstUnmatched;
   }
}


void _seekCycle(
   const _State & st,
   std::vector<PersonType> & cycle,
   std::vector<_RankType> & second,
   _RankType & firstInCycle,
   _RankType & lastInCycle,
   PersonType firstUnmatched,
   std::vector<bool> & tail)
{
   std::vector<bool> cycle_set;
   PersonType person = 0;
   PersonType nextChoice = 0;
   _RankType posInCycle = 0;

   if (firstInCycle > 0)
   {
      assert((firstInCycle - 1) < cycle.size());
      person = cycle[firstInCycle - 1]; // LAST PERSON IN PREVIOUS TAIL
      posInCycle = firstInCycle - 1; // HIS SECOND CHOICE MAY HAVE TO BE UPDATED
      cycle_set = tail;
   }
   else
   {
      posInCycle = 0;
      person = firstUnmatched;
      cycle_set.resize(st.n, false);
   }

   do // GENERATE SEQUENCE
   {
      // UPDATE SECOND CHOICE FOR CURRENT PERSON
      _RankType posInList = second[person];
      do
      {
         nextChoice = st.preference[person][posInList];
         ++posInList;
      } // While person is rejected by nextChoice;
      while (st.ranking[nextChoice][person] > st.rightmost[nextChoice]);
      second[person] = posInList - 1;

      assert(posInCycle < cycle.size());
      cycle_set[person] = true; // Add to set;
      cycle[posInCycle] = person;
      ++posInCycle;
      person = st.worstPotentialPartnerFor(nextChoice);
   } // While no cycle detected;
   while (!cycle_set[person]);

   lastInCycle = posInCycle - 1;
   do // WORK BACK TO BEGINNING OF CYCLE
   {
      --posInCycle;
      cycle_set[cycle[posInCycle]] = false; // Erase from set;
   }
   while (cycle[posInCycle] != person);
   firstInCycle = posInCycle;
   tail = cycle_set;
}


bool _phase2Reduce(
   _State & st,
   const std::vector<PersonType> & cycle,
   std::vector<_RankType> & second,
   _RankType firstInCycle,
   _RankType lastInCycle
)
{
   _RankType rank = 0;

   for (rank = firstInCycle; rank <= lastInCycle; ++rank)
   { // ALLOW NEXT PERSON IN CYCLE TO BE REJECTED
      const PersonType proposer = cycle[rank];
      st.leftmost[proposer] = second[proposer];
      second[proposer] = st.leftmost[proposer] + 1; // PROPER UPDATE UNNECESSARY AT THIS STAGE
      const PersonType nextChoice = st.bestPotentialPartnerFor(proposer);
      st.rightmost[nextChoice] = st.ranking[nextChoice][proposer]; // NEXT_CHOICE HOLDS PROPOSER
   }
   rank = firstInCycle;

   bool solutionPossible = true;
   while ((rank <= lastInCycle) && solutionPossible)
   { // CHECK NO-ONE HAS RUN OUT OF POTENTIAL PARTNERS
      const PersonType proposer = cycle[rank];
      solutionPossible = (st.leftmost[proposer] <= st.rightmost[proposer]);
      ++rank;
   }
   return solutionPossible;
}


} // anonymous namespace;


bool resolve(
   Matrix<PersonType> & preference,
   std::vector<PersonType> & partners
)
{
   partners.clear();
   if (!preference.size() || (preference.size() % 2))
   {
      return false;
   }

   // Initialization;
   _State st(preference);

   bool solutionPossible = _phase1Reduce(st);

   std::vector<_RankType> second(st.n, 0);
   for (PersonType person = 0; person < st.n; ++person)
   {
      second[person] = st.leftmost[person] + 1; // PROPER INITIALISATION UNNECESSARY
   }

   bool solutionFound = false;
   PersonType firstUnmatched = 0;
   std::vector<PersonType> cycle(st.n + 1, 0);
   _RankType firstInCycle = 0;
   _RankType lastInCycle = 0;
   std::vector<bool> tail(st.n, false);
   while (solutionPossible && !solutionFound)
   {
      _findFirstUnmatched(st, firstUnmatched);
      if (firstUnmatched >= st.n)
      {
         solutionFound = true;
      }
      else
      {
         _seekCycle(st, cycle, second, firstInCycle, lastInCycle,
            firstUnmatched, tail
         );
         solutionPossible = _phase2Reduce(st, cycle, second, firstInCycle,
            lastInCycle
         );
      }
   }
   if (solutionFound)
   {
      partners.reserve(st.n);
      for (PersonType person = 0; person < st.n; ++person)
      {
         partners.push_back(st.bestPotentialPartnerFor(person));
      }
   }
   return solutionFound;
}


} // namespace roommates


} // namespace algo
