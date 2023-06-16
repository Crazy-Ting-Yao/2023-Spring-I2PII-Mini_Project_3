#include "./alphabeta.hpp"

#include <cstdlib>

#include "../state/state.hpp"

Move Alpha_Beta::get_move(State *state, int depth) {
    int best_value = -1e9;
    Move best_move;
    for (auto move : state->legal_actions) {
        State* next_state = state->next_state(move);
        int value = recursion(next_state, depth, -1e9, 1e9);
        if (value > best_value) {
            best_value = value;
            best_move = move;
        }
        else if(value == best_value){
            if(rand()%2) best_move = move;
        }
    }
    return best_move;
}

int Alpha_Beta::recursion(State *state, int depth, int alpha, int beta) {
    if (depth == 0) {
        return state->evaluate();
    }
    int best_value = -1e9;
    for (auto move : state->legal_actions) {
        State* next_state = state->next_state(move);
        int value = recursion(next_state, depth - 1, -1*beta, -1*alpha);
        if (value > best_value) {
            best_value = value;
        }
        if (best_value > 4e8){
            return -5e8;
        }
        alpha = std::max(alpha, best_value);
        if (alpha >= beta){
            break;
        }
    }
    return (-1) * best_value;
}