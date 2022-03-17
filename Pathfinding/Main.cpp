#include "Grid.h"


int main()
{
    
    Grid map{ MAP_WIDTH, MAP_HEIGHT };

    map.print();

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Path Finding");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        
        // DRAWING
        map.draw(window);

        window.display();
    }
}