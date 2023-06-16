#include "./minimax.hpp"

#include <cstdlib>

#include "../state/state.hpp"

Move MiniMax::get_move(State *state, int depth) {
    int best_value = -1e9;
    Move best_move;
    for (auto move : state->legal_actions) {
        auto next_state = state->next_state(move);
        int value = recursion(next_state, depth);
        if (value > best_value) {
            best_value = value;
            best_move = move;
        }
    }
    return best_move;
}

int MiniMax::recursion(State *state, int depth) {
    if (depth == 0) {
        return state->evaluate();
    }
    int best_value = -1e9;
    for (auto move : state->legal_actions) {
        State* next_state = state->next_state(move);
        int value = recursion(next_state, depth - 1);
        if (value > best_value) {
            best_value = value;
        }
        if (best_value > 4e8){
            return -1 * best_value;
        }
    }
    return (-1) * best_value;
}