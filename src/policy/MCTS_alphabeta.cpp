#include "./MCTS_alphabeta.hpp"

#include <unistd.h>

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>

#include "../state/state.hpp"
int now_player;

int totalmoves = 52;

Move MCTS_alphabeta::get_move(State* state, int limit) {
    totalmoves -= 2;
    if(totalmoves >= 25) return AlphaBeta::get_move(state, 7);
    now_player = state->player;
    Node* root = new Node(state);
    root->get_legal_actions();
    root->expand();
    while (limit--) {
        Node* node = root;
        do {
            node->visit_times++;
            node = node->select();
        } while (node->children.size() > 0);
        if (node->visit_times) {
            node->visit_times++;
            if (node->game_state == WIN) {
                node->backpropagate(node->player == now_player);
                continue;
            }
            node->expand();
            node = node->select();
        }
        node->visit_times++;
        node->backpropagate(node->simulation());
    }
    Move best_move = root->children[0]->move;
    int best = root->children[0]->visit_times;
    for (auto child : root->children) {
        if (child->visit_times > best) {
            best = child->visit_times;
            best_move = child->move;
        } 
    }
    delete root;
    return best_move;
}

bool Node::simulation() {
    State* state = new State(*this);
    state->get_legal_actions();
    int maximum_steps = totalmoves - now_player;
    while (state->game_state != WIN && maximum_steps--) {
        srand((unsigned)time(NULL));
        int index = rand() % state->legal_actions.size();
        State* next_state = state->next_state(state->legal_actions[index]);
        delete state;
        state = next_state;
        if (state->legal_actions.empty()) state->get_legal_actions();
    }
    int winner;
    if (state->game_state == WIN)
        winner = state->player;
    else
        winner = MCTS_alphabeta::evaluation(state) > 0 ? state->player : 1 - state->player;
    delete state;
    return winner == now_player;
}

void Node::backpropagate(bool win) {
    Node* node = this;
    while (node->parent != nullptr) {
        node->update(win);
        node = node->parent;
    }
}

void Node::update(bool win) {
    if (win) this->win_times++;
    if (this->player != now_player)
        this->ucb =
            (double)this->win_times / this->visit_times +
            sqrt(2 * log(this->parent->visit_times) / this->visit_times);
    else
        this->ucb =
            (double)(this->visit_times - this->win_times) / this->visit_times +
            sqrt(2 * log(this->parent->visit_times) / this->visit_times);
}

void Node::expand() {
    if (this->legal_actions.empty()) this->get_legal_actions();
    if (this->game_state == WIN) return;
    for (auto move : this->legal_actions) {
        State* next_state = this->next_state(move);
        Node* child = new Node(next_state, this, move);
        delete next_state;
        this->children.push_back(child);
    }
    std::sort(this->children.begin(), this->children.end(), cmp());
}

Node* Node::select() {
    Node* node = nullptr;
    double ucb = -1e9;
    for (auto child = this->children.begin(); child != this->children.end();
         child++) {
        if ((*child)->ucb > ucb) {
            node = *child;
            ucb = (*child)->ucb;
        }
        if (ucb == 1e9) return node;
    }
    return node;
}

enum {
    PAWN = 2,
    ROOK = 6,
    KNIGHT = 7,
    BISHOP = 8,
    QUEEN = 20,
    KING = 100
};


bool MCTS_alphabeta::evaluation(State* state) {
    int value = 0;
    for (int i = 0; i < BOARD_H; i++ ) {
        for (int j = 0; j < BOARD_W; j++ ) {
            switch (state->board.board[state->player][i][j]) {
                case 1:
                    value += PAWN;
                    break;
                case 2:
                    value += ROOK;
                    break;
                case 3:
                    value += KNIGHT;
                    break;
                case 4:
                    value += BISHOP;
                    break;
                case 5:
                    value += QUEEN;
                    break;
                case 6:
                    value += KING;
                    break;
                default:
                    break;
            }
            switch (state->board.board[1-state->player][i][j]) {
                case 1:
                    value -= PAWN;
                    break;
                case 2:
                    value -= ROOK;
                    break;
                case 3:
                    value -= KNIGHT;
                    break;
                case 4:
                    value -= BISHOP;
                    break;
                case 5:
                    value -= QUEEN;
                    break;
                case 6:
                    value -= KING;
                    break;
                default:
                    break;
            }
        }
    }
    return value>0;
}

Move AlphaBeta::get_move(State *state, int depth) {
    Move best_move;
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
int AlphaBeta::recursion(State *state, const int depth, const bool player, int alpha, int beta) {
    if(state->game_state == WIN)
        return player ? INF : NEGINF;
    if (!depth) {
        return player ? state->evaluate() : -state->evaluate();
    }
    if (player) {
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