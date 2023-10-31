#include "minimax.h"

int MinimaxSolver::max_value(const GameState &state, Action &best_action) const
{
    int best_score = -2;
    for (const Action &action : state.get_legal_actions())
    {
        Action next_action(-1, -1);
        int score = value(state.get_next_state(action), next_action);
        if (score > best_score)
        {
            best_score = score;
            best_action = action;
        }
    }
    return best_score;
}

int MinimaxSolver::min_value(const GameState &state, Action &best_action) const
{
    int best_score = 2;
    for (const Action &action : state.get_legal_actions())
    {
        Action next_action(-1, -1);
        int score = value(state.get_next_state(action), next_action);
        if (score < best_score)
        {
            best_score = score;
            best_action = action;
        }
    }
    return best_score;
}

int MinimaxSolver::value(const GameState &state, Action &best_action) const
{
    best_action = Action(-1, -1);
    if (state.is_terminal())
        return state.evaluate();
    if (state.get_turn() == Player::X)
        return max_value(state, best_action);
    else
        return min_value(state, best_action);
    return 0;
}

MinimaxSolver::MinimaxSolver(GameState state) : best_action(-1, -1)
{
    best_value = value(state, best_action);
}

Action MinimaxSolver::get_best_action() const
{
    return best_action;
}

int MinimaxSolver::get_best_value() const
{
    return best_value;
}
