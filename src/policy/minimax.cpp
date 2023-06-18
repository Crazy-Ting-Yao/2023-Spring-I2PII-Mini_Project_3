#include "./minimax.hpp"

#include <cstdlib>

#include "../state/state.hpp"

Move MiniMax::get_move(State *state, int depth) {
    int best_value = -1e9;
    std::vector<Move> best_moves;
    for (auto move : state->legal_actions) {
        State* next_state = state->next_state(move);
        int value = recursion(next_state, depth - 1);
        if (value > best_value) {
            best_value = value;
            best_moves.clear();
            best_moves.push_back(move);
        }
        else if(value == best_value){
            best_moves.push_back(move);
        }
    }
    srand(time(NULL));
    return best_moves[rand() % best_moves.size()];
}

int MiniMax::recursion(State *state, int depth) {
    if(state->game_state == WIN){
        return -1e9;
    }
    if (depth == 0) {
        return -state->evaluate();
    }
    int best_value = -1e9;
    if(state->game_state == WIN) return -1e9;
    for (auto move : state->legal_actions) {
        State* next_state = state->next_state(move);
        int value = recursion(next_state, depth - 1);
        if (value > best_value) {
            best_value = value;
        }
        if (best_value > 4e8){
            return -1 * best_value;
        }
        delete next_state;
    }
    return (-1) * best_value;
}