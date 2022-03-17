#include <SFML/Graphics.hpp>
#include "Cell.h"
#include <iostream>

int main()
{
    
    Cell cell{};
    std::cout << cell.getChecked();

    return 0;
}