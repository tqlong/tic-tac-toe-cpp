#include "game_state.h"
#include "minimax.h"
#include <iostream>
#include <set>

using namespace std;

Player get_game_mode()
{
    cout << "Choose game mode:" << endl;
    cout << "1. Human vs Human" << endl;
    cout << "2. Human (X) vs Computer (O)" << endl;
    cout << "3. Computer (X) vs Human (O)" << endl;
    int mode = 0;
    while (mode < 1 || mode > 3)
    {
        cout << "Enter your choice (1-3): ";
        cin >> mode;
    }
    switch (mode)
    {
    case 2:
        return Player::O;
    case 3:
        return Player::X;
    default:
        return Player::None;
    }
}

Action get_human_action(const GameState &state)
{
    vector<Action> legal_actions = state.get_legal_actions();
    set<Action> legal_actions_set(legal_actions.begin(), legal_actions.end());

    Action action(-1, -1);
    do
    {
        cout << "Hey, " << state.get_turn() << ", enter your action (x y): ";
        int x, y;
        cin >> x >> y;
        action = Action(x - 1, y - 1);
    } while (legal_actions_set.find(action) == legal_actions_set.end());
    return action;
}

void render_game_text_mode(const GameState &state)
{
    const Board &board = state.get_board();
    for (int y = 0; y < board[0].size(); y++)
    {
        for (int x = 0; x < board.size(); x++)
        {
            switch (board[x][y])
            {
            case Player::X:
                cout << "X";
                break;
            case Player::O:
                cout << "O";
                break;
            case Player::None:
                cout << " ";
                break;
            }
            if (x < board.size() - 1)
            {
                cout << "|";
            }
        }
        cout << endl;
        if (y < board[0].size() - 1)
        {
            cout << "-+-+-" << endl;
        }
    }
}

int main_text_mode()
{
    Player computer = get_game_mode();
    GameState state;
    while (!state.is_terminal())
    {
        render_game_text_mode(state);
        Player current = state.get_turn();
        bool is_human_turn = current != computer;
        Action action;
        if (!is_human_turn)
        {
            MinimaxSolver solver(state);
            action = solver.get_best_action();
        }
        else
        {
            action = get_human_action(state);
        }
        cout << current << " plays " << action << endl;
        state = state.get_next_state(action);
    }
    render_game_text_mode(state);

    int eval = state.evaluate();
    cout << "Game over!" << endl;
    cout << "Result: " << (eval > 0 ? "X wins!" : (eval < 0 ? "O wins!" : "Draw!")) << endl;
    return 0;
}