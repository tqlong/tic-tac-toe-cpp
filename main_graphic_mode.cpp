#include <SFML/Graphics.hpp>
#include <iostream>
#include <set>
#include "game_state.h"
#include "minimax.h"
using namespace std;

#define CELL_SIZE 400
#define WINDOW_WIDTH (CELL_SIZE*3)
#define WINDOW_HEIGHT (CELL_SIZE*3)

Action get_action_from_mouse_click(int x, int y)
{
    return Action(x/CELL_SIZE, y/CELL_SIZE);
}

void render_game_graphic(const GameState& state, sf::RenderWindow& window, bool thinking = false)
{
    sf::Font font;
    font.loadFromFile("arial.ttf");

    window.clear();
    const Board& board = state.get_board();
    for (int y = 0; y < board[0].size(); y++)
    {
        for (int x = 0; x < board.size(); x++)
        {
            sf::RectangleShape rectangle(sf::Vector2f(CELL_SIZE, CELL_SIZE));
            rectangle.setPosition(x*CELL_SIZE, y*CELL_SIZE);
            rectangle.setOutlineColor(sf::Color::Black);
            rectangle.setOutlineThickness(2);
            rectangle.setFillColor(sf::Color::White);

            window.draw(rectangle);
            sf::Text text((board[x][y] == Player::X) ? "X" : "O", font, 200);
            text.setFillColor(sf::Color::Black);
            sf::FloatRect textRect = text.getLocalBounds();
            text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
            text.setPosition(x*CELL_SIZE + CELL_SIZE/2.0f, y*CELL_SIZE + CELL_SIZE/2.0f);
            if (board[x][y] == Player::X || board[x][y] == Player::O)
                window.draw(text);
        }
    }

    if (thinking) {
        sf::Text text("Thinking ...", font, 160);
        text.setFillColor(sf::Color(255, 0, 0, 128));
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f - CELL_SIZE/2.0f);
        text.setPosition(WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f);
        window.draw(text);
    }

    if (state.is_terminal()) {
        int evaluation = state.evaluate();
        sf::Text text((evaluation == 0) ? "Tie!" : ((evaluation > 0) ? "X wins!" : "O wins!"), font, 320);
        text.setFillColor(sf::Color(255, 0, 0, 128));
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f - CELL_SIZE/2.0f);
        text.setPosition(WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f);
        window.draw(text);
    }
    window.display();
}

Player choose_game_mode_screen(sf::RenderWindow& window)
{
    sf::Font font;
    font.loadFromFile("arial.ttf");

    sf::Text text("Choose game mode:", font, 64);
    text.setFillColor(sf::Color::Red);
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);
    text.setPosition(WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f - 100);

    sf::Text text1("1. Human vs Human", font, 64);
    text1.setFillColor(sf::Color::Red);
    sf::FloatRect textRect1 = text1.getLocalBounds();
    text1.setOrigin(textRect1.left + textRect1.width/2.0f, textRect1.top  + textRect1.height/2.0f);
    text1.setPosition(WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f);

    sf::Text text2("2. Human (X) vs Computer (O)", font, 64);
    text2.setFillColor(sf::Color::Red);
    sf::FloatRect textRect2 = text2.getLocalBounds();
    text2.setOrigin(textRect2.left + textRect2.width/2.0f, textRect2.top  + textRect2.height/2.0f);
    text2.setPosition(WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f + 100);

    sf::Text text3("3. Computer (X) vs Human (O)", font, 64);
    text3.setFillColor(sf::Color::Red);
    sf::FloatRect textRect3 = text3.getLocalBounds();
    text3.setOrigin(textRect3.left + textRect3.width/2.0f, textRect3.top  + textRect3.height/2.0f);
    text3.setPosition(WINDOW_WIDTH/2.0f, WINDOW_HEIGHT/2.0f + 200);

    Player computer = Player::None;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Left) {
                if (text1.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    return Player::None;
                } else if (text2.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    return Player::O;
                } else if (text3.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    return Player::X;
                }
            }
            if (event.type == sf::Event::MouseMoved) {
                    text1.setFillColor(sf::Color::Red);
                    text2.setFillColor(sf::Color::Red);
                    text3.setFillColor(sf::Color::Red);
                if (text1.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y)) {
                    text1.setFillColor(sf::Color::Blue);
                }
                if (text2.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y)) {
                    text2.setFillColor(sf::Color::Blue);
                }
                if (text3.getGlobalBounds().contains(event.mouseMove.x, event.mouseMove.y)) {
                    text3.setFillColor(sf::Color::Blue);
                }
            }
        }
        window.clear();
        window.draw(text);
        window.draw(text1);
        window.draw(text2);
        window.draw(text3);
        window.display();
    }
    return Player::None;
}

int play_game_screen(sf::RenderWindow& window, Player computer)
{
    sf::Clock clock;
    GameState state;
    render_game_graphic(state, window);
    while (window.isOpen())
    {
        sf::Event event;
        Action action;
        Player current = state.get_turn();
        bool is_human_turn = (current != computer);
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed
                && event.mouseButton.button == sf::Mouse::Left
                && is_human_turn)
            {
                action = get_action_from_mouse_click(event.mouseButton.x, event.mouseButton.y);
                if (state.is_valid_action(action) && !state.is_terminal()) {
                    state = state.get_next_state(action);
                }
            }
        }
        float dt = clock.restart().asSeconds();
        if (!is_human_turn && !state.is_terminal()) {
            // render thinking animation
            render_game_graphic(state, window, true);
            MinimaxSolver solver(state);
            action = solver.get_best_action();
            state = state.get_next_state(action);
        }

        render_game_graphic(state, window);
    }
    return 0;
}

int main_graphic_mode()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tic-tac-toe!");

    Player computer = choose_game_mode_screen(window);
    return play_game_screen(window, computer);
}
