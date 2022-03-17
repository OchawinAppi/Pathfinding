#include <SFML/Graphics.hpp>
#include "Grid.h"


int main()
{
    
    Grid map{ 16, 9 };
    
    std::cout << map;

    return 0;
}