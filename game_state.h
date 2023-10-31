#ifndef __GAME_STATE_H
#define __GAME_STATE_H

#include <vector>
#include <ostream>

enum class Player
{
    X,
    O,
    None
};

std::ostream& operator<<(std::ostream& os, const Player& player);

typedef std::vector<std::vector<Player>> Board;

struct Action
{
    int x;
    int y;
    Action(int x, int y) : x(x), y(y) {}
};

std::ostream& operator<<(std::ostream& os, const Action& action);
bool operator<(const Action& a1, const Action& a2);

// current game state = board + turn
class GameState
{
private:
    Board board;
    Player turn;
public:
    GameState();
    // get current turn
    Player get_turn() const;
    // get current board
    const Board& get_board() const;
    // get legal actions
    std::vector<Action> get_legal_actions() const;
    // get next state after action
    GameState get_next_state(const Action& action) const;
    // check if game is over (final state)
    bool is_terminal() const;
    // final state evaluation
    int evaluate() const;
};

#endif