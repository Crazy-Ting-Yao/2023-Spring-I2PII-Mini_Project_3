#include "./submission.hpp"
#include <cstdlib>
#include <vector>
#include <ctime>
#include "../state/state.hpp"

Move Alpha_Beta::get_move(State *state, int depth) {
    Move best_move = state->legal_actions[0];
    int alpha = NEGINF;
    for (auto move : state->legal_actions) {
        State* next_state = state->next_state(move);
        int value = recursion(next_state, depth, false, alpha, INF);
        if (value > alpha) {
            alpha = value;
            best_move = move;
        }
    }
    delete state;
    return best_move;
}
int Alpha_Beta::recursion(State *state, const int depth, const bool isMax, int alpha, int beta) {
    if(state->game_state == WIN)
        return isMax ? INF : NEGINF;
    if (!depth) {
        return isMax ? state->evaluate() : -state->evaluate();
    }
    if (isMax) {
        int ret = NEGINF;
        for (auto move : state->legal_actions) {
            State* next_state = state->next_state(move, (depth != 1));
            int value = recursion(next_state, depth - 1, false, alpha, beta);
            delete next_state;
            if (value > ret) {
                ret = value;
                if (value > alpha) {
                    alpha = value;
                    if (alpha >= beta) {
                        return alpha;
                    }
                }
            }
        }
        return ret;
    }
    else {
        int ret = INF;
        for (auto move : state->legal_actions) {
            State* next_state = state->next_state(move, (depth != 1));
            int value = recursion(next_state, depth - 1, true,  alpha, beta);
            delete next_state;
            if (value < ret) {
                ret = value;
                if (value < beta) {
                    beta = value;
                    if (alpha >= beta) {
                        return beta;
                    }
                }
            }
        }
        return ret;
    }
}