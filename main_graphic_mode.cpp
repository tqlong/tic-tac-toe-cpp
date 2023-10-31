#include <SFML/Graphics.hpp>
#include <iostream>
#include <set>

#include "game_state.h"
#include "minimax.h"
using namespace std;

#define CELL_SIZE 400
#define WINDOW_WIDTH (CELL_SIZE * 3)
#define WINDOW_HEIGHT (CELL_SIZE * 3)

Action get_action_from_mouse_click(int x, int y)
{
    return Action(x / CELL_SIZE, y / CELL_SIZE);
}

sf::Text prepare_text(const string &str,
                      const sf::Font &font,
                      int size,
                      const sf::Color &color,
                      float x,
                      float y)
{
    sf::Text text(str, font, size);
    text.setFillColor(color);
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f,
                   textRect.top + textRect.height / 2.0f);
    text.setPosition(x, y);
    return text;
}

sf::RectangleShape prepare_cell(sf::Vector2f position)
{
    sf::RectangleShape rectangle(sf::Vector2f(CELL_SIZE, CELL_SIZE));
    rectangle.setPosition(position);
    rectangle.setFillColor(sf::Color::White);
    rectangle.setOutlineThickness(2);
    rectangle.setOutlineColor(sf::Color::Black);
    return rectangle;
}

void render_game_graphic(const GameState &state,
                         sf::RenderWindow &window,
                         bool thinking = false)
{
    sf::Font font;
    font.loadFromFile("arial.ttf");

    window.clear();
    const Board &board = state.get_board();
    for (int y = 0; y < board[0].size(); y++)
    {
        for (int x = 0; x < board.size(); x++)
        {
            window.draw(prepare_cell(sf::Vector2f(x * CELL_SIZE, y * CELL_SIZE)));
            if (board[x][y] != Player::None)
            {
                window.draw(prepare_text(
                    (board[x][y] == Player::X) ? "X" : "O", font, 200, sf::Color::Black,
                    x * CELL_SIZE + CELL_SIZE / 2.0f, y * CELL_SIZE + CELL_SIZE / 2.0f));
            }
        }
    }

    if (thinking)
    {
        window.draw(prepare_text(
            "Thinking ...", font, 160, sf::Color(255, 0, 0, 128),
            WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f));
    }

    if (state.is_terminal())
    {
        int evaluation = state.evaluate();
        window.draw(prepare_text(
            (evaluation == 0) ? "Tie!" : ((evaluation > 0) ? "X wins!" : "O wins!"),
            font, 320, sf::Color(255, 0, 0, 128), WINDOW_WIDTH / 2.0f,
            WINDOW_HEIGHT / 2.0f));
    }
    window.display();
}

Player choose_game_mode_screen(sf::RenderWindow &window)
{
    sf::Font font;
    font.loadFromFile("arial.ttf");

    sf::Text text = prepare_text("Choose game mode:", font, 64, sf::Color::Red,
                                 WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f - 100);

    sf::Text text1 = prepare_text("1. Human vs Human", font, 64, sf::Color::Red,
                                  WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);

    sf::Text text2 =
        prepare_text("2. Human (X) vs Computer (O)", font, 64, sf::Color::Red,
                     WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f + 100);

    sf::Text text3 =
        prepare_text("3. Computer (X) vs Human (O)", font, 64, sf::Color::Red,
                     WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f + 200);

    vector<Player> computers = {Player::None, Player::O, Player::X};
    vector<sf::Text *> texts = {&text1, &text2, &text3};

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left)
            {
                int i = 0;
                for (auto text : texts)
                {
                    if (text->getGlobalBounds().contains(event.mouseButton.x,
                                                         event.mouseButton.y))
                    {
                        return computers[i];
                    }
                    i++;
                }
            }
            if (event.type == sf::Event::MouseMoved)
            {
                for (auto text : texts)
                {
                    text->setFillColor(sf::Color::Red);
                    if (text->getGlobalBounds().contains(event.mouseMove.x,
                                                         event.mouseMove.y))
                        text->setFillColor(sf::Color::Blue);
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

int play_game_screen(sf::RenderWindow &window, Player computer)
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
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left && is_human_turn)
            {
                action = get_action_from_mouse_click(event.mouseButton.x,
                                                     event.mouseButton.y);
                if (state.is_valid_action(action) && !state.is_terminal())
                {
                    state = state.get_next_state(action);
                }
            }
        }
        float dt = clock.restart().asSeconds();
        if (!is_human_turn && !state.is_terminal())
        {
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
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
                            "Tic-tac-toe!");

    Player computer = choose_game_mode_screen(window);
    return play_game_screen(window, computer);
}
