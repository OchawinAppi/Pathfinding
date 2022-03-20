#include "Grid.h"
#include "utils.h"
#include "A-Star-Functions.h"


int main()
{

    Grid map{ MAP_WIDTH, MAP_HEIGHT };

    map.print();

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Path Finding");

    sf::Vector2i mousePosition;
    bool pathUpdate = true;
    std::vector<Cell*> path{};

    int run_count = 0;
    bool diag = true;

    while (window.isOpen())
    {
        float xRatio = static_cast<float>(window.getSize().x) / WINDOW_WIDTH;
        float yRatio = static_cast<float>(window.getSize().y) / WINDOW_HEIGHT;
        mousePosition = sf::Mouse::getPosition(window);

        mousePosition.x -= static_cast<int>((static_cast<float>(window.getSize().x) / 2) - static_cast<float>((MAP_WIDTH / 2)) * DEFAULT_TILE_SIZE * xRatio - (DEFAULT_TILE_SIZE * xRatio));
        mousePosition.y -= static_cast<int>((static_cast<float>(window.getSize().y) / 2) - static_cast<float>((MAP_HEIGHT / 2)) * DEFAULT_TILE_SIZE * yRatio - (DEFAULT_TILE_SIZE * xRatio));

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
            if (event.type == sf::Event::KeyPressed)
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                {
                    diag = !diag;
                    pathUpdate = true;
                }
            }
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
                    pathUpdate = true;
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

                    if (map.at(mouseGridPos).isPath)
                    {
                        pathUpdate = true;
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                {
                    pathUpdate = map.moveA(viToVf(mouseGridPos));
                    
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
                {
                    pathUpdate = map.moveB(viToVf(mouseGridPos));
                }
            }
        }

        // CLEAR SCENE
        window.clear();
        
        // DRAWING
        map.draw(window);

        if (map.getA().x != -1 && map.getB().x != -1 && pathUpdate)
        {
            path = a_star(map, map.getA(), map.getB(), diag);
            pathUpdate = false;
        }


        map.drawPath(window, path);
        
        window.display();
        run_count++;
    }
    map.print();
}