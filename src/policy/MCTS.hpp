#pragma once
#include <vector>

#include "../state/state.hpp"
/**
 * @brief Policy class for random policy,
 * your policy class should have get_move method
 */
class MCTS {
   public:
    static Move get_move(State* state, int limit);
    static bool winner_calc(State* state);
    static bool evaluation(State* state);
};

class Node : public State {
   public:
    Node* parent;
    std::vector<Node*> children = {};
    int visit_times = 0;
    int state_value = 0;
    int win_times = 0;
    double ucb = 1e9;
    Move move;
    Node(State* state, Node* parent = nullptr, Move move = Move())
        : State(*state), parent(parent), move(move){
            state_value = evaluate();
        };
    ~Node(){
        if(this->children.size())
            for (auto child : this->children) 
                delete child;
    }
    void expand();
    Node* select();
    void backpropagate(bool win);
    void update(bool win);
    bool simulation();
    void calc_ucb();
};

struct cmp
{
    bool operator()(const Node* a,const Node* b)
    {
        return a->state_value > b->state_value;
    }
};
