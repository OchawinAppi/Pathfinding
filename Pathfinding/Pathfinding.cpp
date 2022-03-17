#include <SFML/Graphics.hpp>
#include "Cell.h"


int main()
{
    
    Cell cell{};
    std::cout << cell.getChecked();

    Direction direction{ 2 };
    std::cout << direction;

    return 0;
}