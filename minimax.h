#ifndef __MINIMAX_H
#define __MINIMAX_H

#include "game_state.h"
class MinimaxSolver
{
    // max value
    int max_value(const GameState &state, Action &best_action) const;
    // min value
    int min_value(const GameState &state, Action &best_action) const;
    // state value
    int value(const GameState &state, Action &best_action) const;
    // best action
    Action best_action;
    // best value
    int best_value;

public:
    MinimaxSolver(GameState state);
    Action get_best_action() const;
    int get_best_value() const;
};

#endif