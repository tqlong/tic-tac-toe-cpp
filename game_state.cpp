#include "game_state.h"

using namespace std;

GameState::GameState()
    : board(3, vector<Player>(3, Player::None)), turn(Player::X)
{
}

Player GameState::get_turn() const
{
    return turn;
}

const Board &GameState::get_board() const
{
    return board;
}

std::vector<Action> GameState::get_legal_actions() const
{
    vector<Action> legal_actions;
    for (int x = 0; x < board.size(); x++)
    {
        for (int y = 0; y < board[x].size(); y++)
        {
            if (board[x][y] == Player::None)
            {
                legal_actions.push_back(Action(x, y));
            }
        }
    }
    return legal_actions;
}

bool GameState::is_valid_action(const Action &action) const
{
    if (action.x < 0 || action.x >= board.size() ||
        action.y < 0 || action.y >= board[0].size())
    {
        return false;
    }
    return board[action.x][action.y] == Player::None;
}

GameState GameState::get_next_state(const Action &action) const
{
    if (!is_valid_action(action))
        return *this;

    GameState next_state = *this;
    next_state.board[action.x][action.y] = turn;
    next_state.turn = (turn == Player::X) ? Player::O : Player::X;
    return next_state;
}

bool GameState::is_terminal() const
{
    // check rows
    for (int x = 0; x < board.size(); x++)
    {
        if (board[x][0] != Player::None &&
            board[x][0] == board[x][1] &&
            board[x][1] == board[x][2])
        {
            return true;
        }
    }
    // check columns
    for (int y = 0; y < board[0].size(); y++)
    {
        if (board[0][y] != Player::None &&
            board[0][y] == board[1][y] &&
            board[1][y] == board[2][y])
        {
            return true;
        }
    }
    // check diagonals
    if (board[0][0] != Player::None &&
        board[0][0] == board[1][1] &&
        board[1][1] == board[2][2])
    {
        return true;
    }
    if (board[0][2] != Player::None &&
        board[0][2] == board[1][1] &&
        board[1][1] == board[2][0])
    {
        return true;
    }
    // check draw
    for (int x = 0; x < board.size(); x++)
    {
        for (int y = 0; y < board[x].size(); y++)
        {
            if (board[x][y] == Player::None)
            {
                return false;
            }
        }
    }
    return true;
}

int GameState::evaluate() const
{
    // check rows
    for (int x = 0; x < board.size(); x++)
    {
        if (board[x][0] != Player::None &&
            board[x][0] == board[x][1] &&
            board[x][1] == board[x][2])
        {
            return (board[x][0] == Player::X) ? 1 : -1;
        }
    }
    // check columns
    for (int y = 0; y < board[0].size(); y++)
    {
        if (board[0][y] != Player::None &&
            board[0][y] == board[1][y] &&
            board[1][y] == board[2][y])
        {
            return (board[0][y] == Player::X) ? 1 : -1;
        }
    }
    // check diagonals
    if (board[0][0] != Player::None &&
        board[0][0] == board[1][1] &&
        board[1][1] == board[2][2])
    {
        return (board[0][0] == Player::X) ? 1 : -1;
    }
    if (board[0][2] != Player::None &&
        board[0][2] == board[1][1] &&
        board[1][1] == board[2][0])
    {
        return (board[0][2] == Player::X) ? 1 : -1;
    }
    // return 0 (draw) here because it is a final state
    return 0;
}

ostream &operator<<(ostream &os, const Player &player)
{
    switch (player)
    {
    case Player::X:
        os << "X";
        break;
    case Player::O:
        os << "O";
        break;
    case Player::None:
        os << " ";
        break;
    }
    return os;
}

ostream &operator<<(ostream &os, const Action &action)
{
    os << "(col " << action.x + 1 << ", row " << action.y + 1 << ")";
    return os;
}

bool operator<(const Action &a1, const Action &a2)
{
    if (a1.x < a2.x)
    {
        return true;
    }
    else if (a1.x > a2.x)
    {
        return false;
    }
    else
    {
        return a1.y < a2.y;
    }
}
