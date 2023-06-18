#pragma once
#include "../state/state.hpp"

#define INF (int)100000
#define NEGINF (int)-100000

/**
 * @brief Policy class for random policy,
 * your policy class should have get_move method
 */
class Alpha_Beta {
   public:
    static Move get_move(State *state, int depth);
    static int recursion(State *state, int depth, bool player, int alpha, int beta);
};