#include <utility>

#include "Headers/grid.h"


// Constructor
Grid::Grid(int x,
           int y)
{
	grid = std::make_unique<std::vector<CellPtrVec>>();

	x += 2;
	y += 2;
	for (int i = 0; i < y; ++i)
	{
		grid->push_back({});
		for (int j = 0; j < x; ++j)
		{
			if (i == 0 || i == y - 1 || j == 0 || j == x - 1)
			{
				grid->at(i).push_back(std::make_unique<Cell>(0, 0, true, j, i));
			}
			else
			{
				grid->at(i).push_back(std::make_unique<Cell>(0, 0, false, j, i));
			}
		}
	}
	aPos = sf::Vector2f(-1.f, -1.f);
	bPos = sf::Vector2f(-1.f, -1.f);
}


// Methods 

// Displaying
void Grid::print() const
{
	for (int i = 0; i < static_cast<int>(grid->size()); i++)
	{
		for (int j = 0; j < static_cast<int>(grid->at(0).size()); j++)
		{
			std::cout << *grid->at(i).at(j);
		}
		std::cout << "\n";
	}
}

// Overloading cout op
std::ostream& operator<<(std::ostream& os,
                         const Grid&   grid)
{
	for (int i = 0; i < static_cast<int>(grid.grid->size()); i++)
	{
		for (int j = 0; j < static_cast<int>(grid.grid->at(0).size()); j++)
		{
			os << grid.grid->at(i).at(j);
		}
		os << "\n";
	}
	return os;
}



// Getting Specific cell ( will need this later )
inline Cell& Grid::at(int x,
                      int y) const
{
	return *grid->at(y).at(x);
}

Cell& Grid::at(const sf::Vector2i& pos) const
{
	return at(pos.x, pos.y);
}

Cell& Grid::at(const sf::Vector2f& pos) const
{
	return at(static_cast<int>(pos.x), static_cast<int>(pos.y));
}

bool Grid::inBounds(int x,
                    int x_size,
                    int y,
                    int y_size)
{
	return x > 0 && x < x_size && (y > 0 && y < y_size);
}

void Grid::initA(const sf::Vector2f& pos)
{
	aPos = sf::Vector2f(pos);
	at(pos).c = 'A';
}

void Grid::initB(const sf::Vector2f& pos)
{
	bPos = sf::Vector2f(pos);
	at(pos).c = 'B';
}

bool Grid::moveA(const sf::Vector2f& pos)
{
	// Shortcircuting
	if (!at(pos).solid && at(pos).c == ' ')
	{
		if (aPos.x == -1.f)
		{
			initA(pos);
			return true;
		}
		at(aPos).makeEmpty();
		aPos.x = pos.x;
		aPos.y = pos.y;
		at(pos).c = 'A';
		return true;
	}
	return false;
}

bool Grid::moveB(const sf::Vector2f& pos)
{
	// Shortcircuting
	if (!at(pos).solid && at(pos).c == ' ')
	{
		if (bPos.x == -1.f)
		{
			initB(pos);
			return true;
		}
		at(bPos).makeEmpty();
		bPos.x = pos.x;
		bPos.y = pos.y;
		at(pos).c = 'B';
		return true;
	}
	return false;
}

bool Grid::canRoute()
{
	return this->getA().x != -1.f && this->getB().x != -1.f;
}

void Grid::resetA()
{
	aPos.x = -1;
	aPos.y = -1;
}

void Grid::resetB()
{
	bPos.x = -1;
	bPos.y = -1;
}

void Grid::resetPath() const
{
	for (int y = 1; y < MAP_HEIGHT + 1; ++y)
	{
		for (int x = 1; x < MAP_WIDTH + 1; ++x)
		{
			this->at(x, y).is_path = false;
		}
	}
}

void Grid::resetGrid()
{
	resetA();
	resetB();

	for (int y = 1; y < MAP_HEIGHT + 1; ++y)
	{
		for (int x = 1; x < MAP_WIDTH + 1; ++x)
		{
			this->at(x, y).f_dist = 0;
			this->at(x, y).g_dist = 0;
			this->at(x, y).h_dist = 0;
			this->at(x, y).solid = false;
			this->at(x, y).c = ' ';
			this->at(x, y).is_path = false;
		}
	}
}


std::vector<Cell*> Grid::getNeighbors(const sf::Vector2f& pos, const bool diag) const
{
	const int x{ static_cast<int>(pos.x) };
	const int y{ static_cast<int>(pos.y) };

	std::vector<Cell*> neighbors{};


	if (diag)
	{
		for (int _x = x - 1; _x < x + 2; ++ _x)
		{
			if (!(_x == x) && this->inBounds(_x, MAP_WIDTH + 1, y, MAP_HEIGHT + 1))
			{
				if (Cell* cell = &at(_x, y); !cell->solid)
				{
					neighbors.push_back(cell);
				}
			}
		}

		for (int _y = y - 1; _y < y + 2; ++_y)
		{
			if (!(_y == y) && this->inBounds(x, MAP_WIDTH + 1, _y, MAP_HEIGHT + 1))
			{
				if (Cell* cell = &at(x, _y); !cell->solid)
				{
					neighbors.push_back(cell);
				}
			}
		}
		return neighbors;
	}

	for (int _x = x - 1; _x < x + 2; _x++)
	{
		for (int _y = y - 1; _y < y + 2; _y++)
		{
			if (!(_x == x && _y == y) && this->inBounds(_x, MAP_WIDTH + 1, _y, MAP_HEIGHT + 1))
			{
				if (Cell* cell = &at(_x, _y); !cell->solid)
				{
					neighbors.push_back(cell);
				}
			}
		}
	}
	return neighbors;
}

sf::Vector2f& Grid::getB()
{
	return this->bPos;
}

sf::Vector2f& Grid::getA()
{
	return this->aPos;
}

inline sf::Vector2f Grid::offsetPosition(const sf::Vector2f& position)
{
	return {
		position.x * DEFAULT_TILE_SIZE + static_cast<float>(WINDOW_WIDTH) / 2.f - MAP_WIDTH / 2.f * DEFAULT_TILE_SIZE -
		DEFAULT_TILE_SIZE,
		position.y * DEFAULT_TILE_SIZE + static_cast<float>(WINDOW_HEIGHT) / 2.f - MAP_HEIGHT / 2.f * DEFAULT_TILE_SIZE
		- DEFAULT_TILE_SIZE
	};
}

std::vector<Cell*>& Grid::getSearched()
{
	return searched;
}

void Grid::addSearched(Cell* cell)
{
	searched.push_back(cell);
}

void Grid::resetSearched()
{
	searched.clear();
	resetPath();
}

void Grid::read(const std::vector<sf::Vector2i>& mapInfo) const
{
	for (int y = 1; y < MAP_HEIGHT + 1; ++y)
	{
		for (int x = 1; x < MAP_WIDTH + 1; ++x)
		{
			this->at(x, y).makeSolid();
		}
	}


	for (const auto& location : mapInfo)
	{
		this->at(location.x, location.y).makeEmpty();
	}
}

void Grid::draw(sf::RenderWindow& window) const
{
	for (int y = 0; y < static_cast<int>(grid->size()); y++)
	{
		for (int x = 0; x < static_cast<int>(grid->at(0).size()); x++)
		{
			grid->at(y).at(x)->draw(window);
		}
	}
}

void Grid::draw(sf::RenderWindow& window,
	const std::vector<Cell*>& path,
	const sf::Color           color,
	sf::Shape&& shape,
	const int                 drawCount,
	const float               scale)
{
	const sf::Vector2f newScale(scale, scale);

	const float delta = (static_cast<float>(DEFAULT_TILE_SIZE) - DEFAULT_TILE_SIZE * scale) / 2;

	shape.setFillColor(color);
	int count = 0;
	for (const auto& tile : path)
	{
		if (drawCount == count++) return;
		if (tile->c != ' ' || tile->solid) continue;
		shape.setScale(newScale);
		shape.setPosition(offsetPosition(tile->pos));
		shape.move(sf::Vector2f(delta, delta));
		window.draw(shape);
	}
}

void Grid::draw(sf::RenderWindow& window,
	const std::vector<Cell*>& path,
	const sf::Uint8           r,
	const sf::Uint8           g,
	const sf::Uint8           b,
	const sf::Uint8           a,
	sf::Shape&& shape,
	const int                 drawCount,
	const float               scale) const
{
	draw(window, path, sf::Color(r, g, b, a), std::forward<sf::Shape>(shape), drawCount, scale);
}



void Grid::draw(sf::RenderWindow&                window,
                const std::vector<sf::Vector2i>& path,
                sf::Uint8                        r,
                sf::Uint8                        g,
                sf::Uint8                        b,
                sf::Uint8                        a,
                sf::Shape&&                      shape,
                int                              drawCount,
                float                            scale) const
{
	this->draw(window, path, sf::Color(r, g, b, a), std::forward<sf::Shape>(shape), drawCount, scale);
}

void Grid::draw(sf::RenderWindow&                window,
                const std::vector<sf::Vector2i>& path,
                sf::Color                        color,
                sf::Shape&&                      shape,
                int                              drawCount,
                float                            scale) const
{
	std::vector<Cell*> temp{};
	for (auto& [x, y] : path)
	{
		temp.push_back(&this->at(x, y));
	}
	this->draw(window, temp, color, std::forward<sf::Shape>(shape), drawCount, scale);
}
