#include "Grid.h"
#include "A-Star-Functions.h"


int main()
{

    Grid map{ MAP_WIDTH, MAP_HEIGHT };



    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Path Finding");

    sf::Vector2i mousePosition;
    std::vector<Cell*> path{};

    int run_count = 0;
    bool diag = true;

    bool pathUpdate = true;
    bool pathBlocked = false; // Cannot route
    sf::Vector2f* enclosedCell{};
    std::vector<Cell*> enclosedCellRoom{};
    char target{};

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
                    pathBlocked = false;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
                {
                    map.resetGrid();
                    enclosedCellRoom.clear();
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
                    pathBlocked = false;
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
                if (pathBlocked && map.canRoute())
                {
                    Cell* A = &map.at(map.getA());
                    Cell* B = &map.at(map.getB());
                    bool containsA = contatinsPtr(enclosedCellRoom, A);
                    bool containsB = contatinsPtr(enclosedCellRoom, B);
                    pathBlocked = !(containsA == containsB);
                }
            }
        }

        // UPDATE SCENE
        if (map.canRoute() && pathUpdate && !pathBlocked)
        {
            enclosedCellRoom = {};
            path = a_star(map, map.getA(), map.getB(), diag);

            // If no path can be made, it is blocked.
            pathBlocked = (path.empty());

            // Get info about what's in the smallest room (whats being blocked)
            if (pathBlocked)
            {

                auto [_enclosedCell, _target, _enclosedCellRoom] = indiscriminateSearch(map, map.getA(), 'B', map.getB(), 'A', diag);
                enclosedCell = nullptr;
                enclosedCell = &_enclosedCell->pos;
                target = _target;
                for (auto var : _enclosedCellRoom)
                {
                    enclosedCellRoom.push_back(var);
                }

                //log("PATH BLOCKED", enclosedCell->x, enclosedCell->y, target, enclosedCellRoom.size());
            }

            pathUpdate = false;
        }

        if (!map.canRoute() || pathBlocked) path = {};

        // CLEAR SCENE
        window.clear();
        
        // DRAWING
        map.draw(window);
        map.draw(window, enclosedCellRoom, 200, 200, 200, 100, sf::CircleShape(DEFAULT_TILE_SIZE / 2.f));
        map.draw(window, path, sf::Color::Yellow, sf::CircleShape(DEFAULT_TILE_SIZE / 2.f));
        
        window.display();
        run_count++;

    }
}