#include "../game_state.h"
#include <gtest/gtest.h>

TEST(GameStateTest, Init)
{
    GameState s;
    const Board &b = s.get_board();

    EXPECT_EQ(b.size(), 3); // board is 3x3
    EXPECT_EQ(b[0].size(), 3);
    EXPECT_EQ(b[1].size(), 3);
    EXPECT_EQ(b[2].size(), 3);

    EXPECT_EQ(b[0][0], Player::None); // board is empty
    EXPECT_EQ(b[0][1], Player::None);
    EXPECT_EQ(b[0][2], Player::None);
    EXPECT_EQ(b[1][0], Player::None);
    EXPECT_EQ(b[1][1], Player::None);
    EXPECT_EQ(b[1][2], Player::None);
    EXPECT_EQ(b[2][0], Player::None);
    EXPECT_EQ(b[2][1], Player::None);
    EXPECT_EQ(b[2][2], Player::None);

    EXPECT_EQ(s.get_turn(), Player::X); // X always goes first
}

TEST(GameState, IsValidAction)
{
    GameState s;
    Action a1(0, 0);
    Action a2(1, 1);
    Action a3(2, 2);
    Action a4(3, 3); // out of bounds

    EXPECT_TRUE(s.is_valid_action(a1)); // empty board
    EXPECT_TRUE(s.is_valid_action(a2));
    EXPECT_TRUE(s.is_valid_action(a3));
    EXPECT_FALSE(s.is_valid_action(a4));
}

TEST(GameState, GetNextState)
{
    GameState s;
    Action a1(0, 0);
    Action a2(1, 1);
    Action a3(2, 2);
    Action a4(3, 3); // out of bounds

    GameState s1 = s.get_next_state(a1);
    GameState s2 = s1.get_next_state(a2);
    GameState s3 = s2.get_next_state(a3);
    GameState s4 = s3.get_next_state(a4); // should not change state

    EXPECT_EQ(s1.get_board()[0][0], Player::X); // X goes first
    EXPECT_EQ(s2.get_board()[1][1], Player::O); // O goes second
    EXPECT_EQ(s3.get_board()[2][2], Player::X); // X goes third
    EXPECT_EQ(s4.get_board()[2][2], Player::X); // should not change state
}

TEST(GameState, IsTerminal)
{
    GameState s;
    Action a1(0, 0);
    Action a2(1, 1);
    Action a3(2, 2);
    Action a4(0, 1);
    Action a5(1, 0);
    Action a6(2, 1);

    GameState s1 = s.get_next_state(a1);
    GameState s2 = s1.get_next_state(a2);
    GameState s3 = s2.get_next_state(a3);
    GameState s4 = s3.get_next_state(a4);
    GameState s5 = s4.get_next_state(a5);
    GameState s6 = s5.get_next_state(a6);

    EXPECT_FALSE(s.is_terminal());  // empty board
    EXPECT_FALSE(s1.is_terminal()); // 1 move
    EXPECT_FALSE(s2.is_terminal()); // 2 moves
    EXPECT_FALSE(s3.is_terminal()); // 3 moves
    EXPECT_FALSE(s4.is_terminal()); // 4 moves
    EXPECT_FALSE(s5.is_terminal()); // 5 moves
    EXPECT_TRUE(s6.is_terminal());  // 6 moves
}