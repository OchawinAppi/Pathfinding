#include "Grid.h"
#include "utils.h"


int main()
{
    
    Grid map{ MAP_WIDTH, MAP_HEIGHT };

    map.print();

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Path Finding");


    sf::Vector2i mousePosition;


    while (window.isOpen())
    {

        mousePosition = sf::Mouse::getPosition(window);
        mousePosition.x -= window.getSize().x / 4;
        mousePosition.y -= window.getSize().y / 4;

        sf::Vector2i mouseGridPos{ (int)(mousePosition.x / DEFAULT_TILE_SIZE), (int)(mousePosition.y / DEFAULT_TILE_SIZE) };


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

        // Drawing map
        if (
            sf::Mouse::isButtonPressed(sf::Mouse::Left) && 
            (map.inBounds((int)(mousePosition.x / DEFAULT_TILE_SIZE), MAP_WIDTH + 1) && map.inBounds((int)(mousePosition.y / DEFAULT_TILE_SIZE), MAP_HEIGHT + 1)) &&
            !map.at((int)(mousePosition.x / DEFAULT_TILE_SIZE), (int)(mousePosition.y / DEFAULT_TILE_SIZE)).solid 
            ) 
        {
            map.at(mouseGridPos).makeSolid();
        }

        if (
            sf::Keyboard::isKeyPressed(sf::Keyboard::A) &&
            (map.inBounds((int)(mousePosition.x / DEFAULT_TILE_SIZE), MAP_WIDTH + 1) && map.inBounds((int)(mousePosition.y / DEFAULT_TILE_SIZE), MAP_HEIGHT + 1)) &&
            !map.at((int)(mousePosition.x / DEFAULT_TILE_SIZE), (int)(mousePosition.y / DEFAULT_TILE_SIZE)).solid
            )
        {
            map.moveA(viToVf(mouseGridPos));
        }

        if (
            sf::Keyboard::isKeyPressed(sf::Keyboard::B) &&
            (map.inBounds((int)(mousePosition.x / DEFAULT_TILE_SIZE), MAP_WIDTH + 1) && map.inBounds((int)(mousePosition.y / DEFAULT_TILE_SIZE), MAP_HEIGHT + 1)) &&
            !map.at((int)(mousePosition.x / DEFAULT_TILE_SIZE), (int)(mousePosition.y / DEFAULT_TILE_SIZE)).solid
            )
        {
            map.moveB(viToVf(mouseGridPos));
        }




        window.clear();
        
        // DRAWING
        map.draw(window);

        window.display();
    }
    map.print();
}