#include <SFML/Graphics.hpp>
#include <iostream>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main()
{
    sf::RenderWindow window(
        sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tic-tac-toe!");


    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            // TODO: process event
        }
        float dt = clock.restart().asSeconds();
        //TODO: update game state after dt seconds

        window.clear();
        // TODO draw game state
        window.display();
    }
    return 0;
}
