#include "Grid.h"
#include "utils.h"
#include "A-Star-Functions.h"


int main()
{

    Grid map{ MAP_WIDTH, MAP_HEIGHT };

    map.print();

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Path Finding");


    sf::Vector2i mousePosition;
    bool mapChanged = false;


    int run_count = 0;
    while (window.isOpen())
    {
        float xRatio = static_cast<float>(window.getSize().x) / WINDOW_WIDTH;
        float yRatio = static_cast<float>(window.getSize().y) / WINDOW_HEIGHT;
        mousePosition = sf::Mouse::getPosition(window);

        mousePosition.x -= static_cast<int>((static_cast<float>(window.getSize().x) / 2) - static_cast<float>((MAP_HEIGHT / 2)) * DEFAULT_TILE_SIZE * xRatio);
        mousePosition.y -= static_cast<int>((static_cast<float>(window.getSize().y) / 2) - static_cast<float>((MAP_HEIGHT / 2)) * DEFAULT_TILE_SIZE * yRatio);

        sf::Vector2i mouseGridPos{ 
            static_cast<int>(mousePosition.x / (DEFAULT_TILE_SIZE * xRatio)), 
            static_cast<int>(mousePosition.y / (DEFAULT_TILE_SIZE * yRatio))
        };

        // PROCESS EVENTS/ INPUTS
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
        }

        // Updating and editting positions
        if (
            (isAnyKeyPressed() || sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Right)) &&
            (map.inBounds(mouseGridPos.x, MAP_WIDTH + 1, mouseGridPos.y, MAP_HEIGHT + 1))
            )
        {
            mapChanged = true;
            if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
            {
                if (char c = map.at(mouseGridPos).c; c != ' ')
                {
                    if (c == 'A')
                    {
                        map.resetA();
                    }
                    else if (c == 'B')
                    {
                        map.resetB();
                    }

                    map.at(mouseGridPos).makeEmpty();
                }
                
            }

            if (!map.at(mouseGridPos).solid)
            {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                {
                    if (char c = map.at(mouseGridPos).c; c != ' ')
                    {
                        if (c == 'A')
                        {
                            map.resetA();
                        }
                        else if (c == 'B')
                        {
                            map.resetB();
                        }
                    }

                    map.at(mouseGridPos).makeSolid();
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                {
                    map.moveA(viToVf(mouseGridPos));
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
                {
                    map.moveB(viToVf(mouseGridPos));
                }
            }
        }

        // CLEAR SCENE
        window.clear();
        
        // DRAWING
        map.draw(window);

        if (map.getA().x != -1 && map.getB().x != -1)
        {
            map.drawPath(window, a_star(map, map.getA(), map.getB()));
        }

        window.display();
        run_count++;
    }
    map.print();
}