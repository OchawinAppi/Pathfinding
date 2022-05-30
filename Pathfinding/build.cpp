#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>

//// CONFIG ////

// Window stuff
constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 800;

// Map stuff
constexpr int MAP_WIDTH = 40;
constexpr int MAP_HEIGHT = 40;

// Tile
constexpr float SIZE_MULTIPLIER = 1.f;
constexpr int   DECREASE_BOX_SIZE = 1;
constexpr float DEFAULT_TILE_SIZE = static_cast<int>(WINDOW_WIDTH) / (MAP_WIDTH + 2 + DECREASE_BOX_SIZE) *
	SIZE_MULTIPLIER;

// How fast nodes are displayed ( Higher value == slower )
constexpr int SEARCHED_DISPLAY_RATE = 1;
constexpr int PATH_CONSTRUCTION_RATE = 15;

//// Utils ////

inline sf::Vector2i vfToVi(const sf::Vector2f& vec)
{
	return { static_cast<int>(vec.x), static_cast<int>(vec.y) };
}

inline sf::Vector2f viToVf(const sf::Vector2i& vec)
{
	return { static_cast<float>(vec.x), static_cast<float>(vec.y) };
}

inline bool isAnyKeyPressed()
{
	for (int k = -1; k < sf::Keyboard::KeyCount; ++k)
	{
		if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(k)))
			return true;
	}
	return false;
}

inline sf::Vector2f offset(const sf::Vector2f& pos)
{
	return {
		pos.x * DEFAULT_TILE_SIZE + static_cast<float>(MAP_WIDTH) / 2 - MAP_WIDTH / 2.f * DEFAULT_TILE_SIZE -
		DEFAULT_TILE_SIZE,
		pos.y * DEFAULT_TILE_SIZE + static_cast<float>(WINDOW_HEIGHT) / 2 - MAP_HEIGHT / 2.f * DEFAULT_TILE_SIZE -
		DEFAULT_TILE_SIZE
	};
}

//// CELL /////

class Cell final
{
public:
	// A-Star attributes
	int          g_dist;
	int          h_dist;
	int          f_dist;
	bool         is_path;
	sf::Vector2f pos;
	bool         solid; ///< Won't evaluate solid cells ( walls )
	char         c;     ///< for printing on console ( pre-SFML ) AND conditional checking!

	// SFML
	sf::RectangleShape tile;

	// Constructors, Destructor
	Cell()
		: g_dist(-1)
		, h_dist(-1)
		, f_dist(-1)
		, is_path(false)
		, pos(-1.f, -1.f)
		, solid(false)
		, c(' ')
		, tile()
	{
	}

	Cell(int  g,
	     int  h,
	     bool solid,
	     int  x,
	     int  y)
		: g_dist(g)
		, h_dist(h)
		, f_dist(g_dist + h_dist)
		, is_path(false)
		, pos(static_cast<float>(x), static_cast<float>(y))
		, solid(solid)
		, c(solid ? '#' : ' ')
		, tile({ DEFAULT_TILE_SIZE, DEFAULT_TILE_SIZE })
	{
		// Note: Multiply by 
		tile.setPosition(
			pos.x * DEFAULT_TILE_SIZE + static_cast<float>(WINDOW_WIDTH) / 2 - MAP_WIDTH / 2.f * DEFAULT_TILE_SIZE -
			DEFAULT_TILE_SIZE,
			pos.y * DEFAULT_TILE_SIZE + static_cast<float>(WINDOW_HEIGHT) / 2 - MAP_HEIGHT / 2.f * DEFAULT_TILE_SIZE -
			DEFAULT_TILE_SIZE
		);
	}

	// Methods
	void makeSolid()
	{
		this->solid = true;
		this->c = '#';
	}

	void makeEmpty()
	{
		this->solid = false;
		this->c = ' ';
	}

	void updateG(const int n)
	{
		this->g_dist = n;
	}

	void updateH(const int n)
	{
		this->h_dist = n;
	}

	void updateF()
	{
		this->f_dist = this->h_dist + this->g_dist;
	}

	void draw(sf::RenderWindow& window)
	{
		if (solid)
		{
			tile.setFillColor(sf::Color(200, 200, 200));
		}
		else
		{
			switch (this->c)
			{
			case 'A': tile.setFillColor(sf::Color::Red);
				break;
			case 'B': tile.setFillColor(sf::Color::Blue);
				break;
			default: tile.setFillColor(sf::Color::Black);
			}
		}
		window.draw(tile);
	}

	void setColor(const sf::Color color)
	{
		this->tile.setFillColor(color);
	}

	// Friends, because we don't want lonely classes ;)
	friend std::ostream& operator<<(std::ostream& os,
	                                const Cell&   cell)
	{
		return os << cell.c;
	}

	bool operator==(const Cell& other) const
	{
		return this->pos == other.pos;
	}

	bool operator!=(const Cell& other) const
	{
		return this != &other;
	}
};

//// GRID ////

class Grid final
{
private:
	using CellPtr = std::unique_ptr<Cell>;
	using CellPtrVec = std::vector<CellPtr>;

	std::unique_ptr<std::vector<CellPtrVec>> grid;


	std::vector<Cell*> searched;
	sf::Vector2f       aPos;
	sf::Vector2f       bPos;
public:
	// Constructor
	Grid(int x,
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

	// Destructor not needed. The compiler got this. ;) ( probably )

	// Displaying - Console
	void print() const
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


	void draw(sf::RenderWindow& window) const
	{
		for (int y = 0; y < static_cast<int>(grid->size()); y++)
		{
			for (int x = 0; x < static_cast<int>(grid->at(0).size()); x++)
			{
				grid->at(y).at(x)->draw(window);
			}
		}
	}

	static void draw(sf::RenderWindow&         window,
	                 const std::vector<Cell*>& path,
	                 sf::Color                 color,
	                 sf::Shape&&               shape,
	                 int                       drawCount,
	                 float                     scale = 1.f)
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

	void draw(sf::RenderWindow&                window,
	          const std::vector<sf::Vector2i>& path,
	          sf::Color                        color,
	          sf::Shape&&                      shape,
	          int                              drawCount,
	          float                            scale = 1.f) const
	{
		std::vector<Cell*> temp{};
		for (auto& [x, y] : path)
		{
			temp.push_back(&this->at(x, y));
		}
		draw(window, temp, color, std::forward<sf::Shape>(shape), drawCount, scale);
	}

	void draw(sf::RenderWindow&         window,
	          const std::vector<Cell*>& path,
	          sf::Uint8                 r,
	          sf::Uint8                 g,
	          sf::Uint8                 b,
	          sf::Uint8                 a,
	          sf::Shape&&               shape,
	          int                       drawCount,
	          float                     scale = 1.f) const
	{
		draw(window, path, sf::Color(r, g, b, a), std::forward<sf::Shape>(shape), drawCount, scale);
	}


	void draw(sf::RenderWindow&                window,
	          const std::vector<sf::Vector2i>& path,
	          sf::Uint8                        r,
	          sf::Uint8                        g,
	          sf::Uint8                        b,
	          sf::Uint8                        a,
	          sf::Shape&&                      shape,
	          int                              drawCount,
	          float                            scale = 1.f) const
	{
		this->draw(window, path, sf::Color(r, g, b, a), std::forward<sf::Shape>(shape), drawCount, scale);
	}


	// Getting Specific cell ( will need this later )
	[[nodiscard]] Cell& at(int x,
	                       int y) const
	{
		return *grid->at(y).at(x);
	}

	[[nodiscard]] Cell& at(const sf::Vector2i& pos) const
	{
		return at(pos.x, pos.y);
	}

	[[nodiscard]] Cell& at(const sf::Vector2f& pos) const
	{
		return at(static_cast<int>(pos.x), static_cast<int>(pos.y));
	}

	static bool inBounds(int x,
	                     int x_size,
	                     int y,
	                     int y_size)
	{
		return x > 0 && x < x_size && (y > 0 && y < y_size);
	}


	// Points 
	void initA(const sf::Vector2f& pos)
	{
		aPos = sf::Vector2f(pos);
		at(pos).c = 'A';
	}

	void initB(const sf::Vector2f& pos)
	{
		bPos = sf::Vector2f(pos);
		at(pos).c = 'B';
	}

	bool moveA(const sf::Vector2f& pos)
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

	bool moveB(const sf::Vector2f& pos)
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

	bool canRoute()
	{
		return this->getA().x != -1.f && this->getB().x != -1.f;
	}

	void resetA()
	{
		aPos.x = -1;
		aPos.y = -1;
	}

	void resetB()
	{
		bPos.x = -1;
		bPos.y = -1;
	}

	void resetPath() const
	{
		for (int y = 1; y < MAP_HEIGHT + 1; ++y)
		{
			for (int x = 1; x < MAP_WIDTH + 1; ++x)
			{
				this->at(x, y).is_path = false;
			}
		}
	}

	void resetGrid()
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

	std::vector<Cell*> getNeighbors(const sf::Vector2f& pos,
	                                const bool          diag) const
	{
		const int x{ static_cast<int>(pos.x) };
		const int y{ static_cast<int>(pos.y) };

		std::vector<Cell*> neighbors{};


		if (diag)
		{
			for (int _x = x - 1; _x < x + 2; ++_x)
			{
				if (!(_x == x) && Grid::inBounds(_x, MAP_WIDTH + 1, y, MAP_HEIGHT + 1))
				{
					if (Cell* cell = &at(_x, y); !cell->solid)
					{
						neighbors.push_back(cell);
					}
				}
			}

			for (int _y = y - 1; _y < y + 2; ++_y)
			{
				if (!(_y == y) && Grid::inBounds(x, MAP_WIDTH + 1, _y, MAP_HEIGHT + 1))
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
				if (!(_x == x && _y == y) && Grid::inBounds(_x, MAP_WIDTH + 1, _y, MAP_HEIGHT + 1))
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

	sf::Vector2f& getB()
	{
		return this->bPos;
	}

	sf::Vector2f& getA()
	{
		return this->aPos;
	}

	static sf::Vector2f offsetPosition(const sf::Vector2f& position)
	{
		return {
			position.x * DEFAULT_TILE_SIZE + static_cast<float>(WINDOW_WIDTH) / 2.f - MAP_WIDTH / 2.f *
			DEFAULT_TILE_SIZE -
			DEFAULT_TILE_SIZE,
			position.y * DEFAULT_TILE_SIZE + static_cast<float>(WINDOW_HEIGHT) / 2.f - MAP_HEIGHT / 2.f *
			DEFAULT_TILE_SIZE
			- DEFAULT_TILE_SIZE
		};
	}

	std::vector<Cell*>& getSearched()
	{
		return searched;
	}

	void addSearched(Cell* cell)
	{
		searched.push_back(cell);
	}

	void resetSearched()
	{
		searched.clear();
		resetPath();
	}

	void read(const std::vector<sf::Vector2i>& mapInfo) const
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
};

// A STAR

inline bool contatinsPtr(const std::vector<Cell*>& vec,
                         Cell*&                    item)
{
	for (int i = 0; i < static_cast<int>(vec.size()); i++) { if (vec.at(i) == item) { return true; } }
	return false;
}

template <typename T>
auto contains(const std::vector<T>& vec,
              const T&              _item)
{
	struct returnInfo
	{
		bool contains;
		int  index;
	};
	for (int i = 0; i < static_cast<int>(vec.size()); i++) { if (vec.at(i) == _item) return returnInfo{ true, i }; }
	return returnInfo{ false, -1 };
}

//// FIND IF TARGET IS REACHABLE ////
auto indiscriminateSearch(const Grid&         map,
                          const sf::Vector2f& startNode1,
                          const char          targetLetter1,
                          const sf::Vector2f& startNode2,
                          const char          targetLetter2,
                          const bool          diag)
{
	struct ReturnInfo
	{
		Cell*              node;
		char               target;
		std::vector<Cell*> room;
	};

	std::vector collectedNodes1{ &map.at(startNode1) };
	std::vector collectedNodes2{ &map.at(startNode2) };

	int count = 0;
	while (static_cast<int>(collectedNodes1.size()) > count && collectedNodes2.size() > static_cast<unsigned>(count))
	{
		const Cell* current_node1 = collectedNodes1.at(count);
		const Cell* current_node2 = collectedNodes2.at(count);

		if (current_node1->c == targetLetter1)
			return ReturnInfo{
				&map.at(startNode1), targetLetter1, collectedNodes1
			};
		if (current_node2->c == targetLetter2)
			return ReturnInfo{
				&map.at(startNode2), targetLetter2, collectedNodes2
			};


		// First one
		const auto& neighbors1 = map.getNeighbors(current_node1->pos, diag);

		if (neighbors1.size() == static_cast<unsigned>(0))
			return ReturnInfo{
				&map.at(startNode1), targetLetter1, collectedNodes1
			};

		// Second one
		const auto& neighbors2 = map.getNeighbors(current_node2->pos, diag);

		if (neighbors2.size() == static_cast<unsigned>(0))
			return ReturnInfo{
				&map.at(startNode2), targetLetter2, collectedNodes2
			};

		for (auto* neighbor : neighbors1)
		{
			if (contatinsPtr(collectedNodes1, neighbor)) continue;
			collectedNodes1.push_back(neighbor);
		}

		for (auto* neighbor : neighbors2)
		{
			if (contatinsPtr(collectedNodes2, neighbor)) continue;
			collectedNodes2.push_back(neighbor);
		}
		++count;
	}

	if (collectedNodes1.size() <= static_cast<unsigned>(count))
		return ReturnInfo{ &map.at(startNode1), targetLetter1, collectedNodes1 };
	return ReturnInfo{ &map.at(startNode2), targetLetter2, collectedNodes2 };
}


//// GET DISTANCE ////
// Note, for simplicity, we covert 1.0 and 1.4 to 10 and 14. We hate decimals.
inline int heuristic(const sf::Vector2f& node,
                     const sf::Vector2f& target)
{
	int x_dist = static_cast<int>(abs(node.x - target.x));
	int y_dist = static_cast<int>(abs(node.y - target.y));

	int distance = 0;
	while (x_dist >= 1 && y_dist >= 1)
	{
		x_dist--;
		y_dist--;
		distance += 14;
	}
	distance += x_dist * 10;
	distance += y_dist * 10;
	return distance;
}

//// A STAR ////
inline std::vector<Cell*> aStar(Grid&               map,
                                const sf::Vector2f& startNode,
                                const sf::Vector2f& targetNode,
                                const bool          diag)
{
	map.resetSearched();
	map.at(startNode).updateG(0);
	map.at(startNode).updateH(heuristic(startNode, targetNode));
	map.at(startNode).updateF();

	std::vector        open_nodes{ &map.at(startNode) };
	std::vector<Cell*> closed_nodes{};

	std::unordered_map<Cell*, Cell*> travel_map;
	std::vector<Cell*>               travel_path{};

	Cell* current_node = nullptr;

	bool quit = false;

	while (!open_nodes.empty() && !quit)
	{
		current_node = open_nodes.at(0);
		int current_node_index = 0;

		// Step 1: Get Lowest node, it'll be our current node.
		for (size_t i = 1; i < open_nodes.size(); ++i)
		{
			auto* temp_node = open_nodes.at(i);

			// Check for greater f_cost, if there are nodes with same f_cost, check h_cost.
			if (temp_node->f_dist < current_node->f_dist)
			{
				current_node = temp_node;
				current_node_index = static_cast<int>(i);
			}
			else if (temp_node->f_dist == current_node->f_dist)
			{
				if (temp_node->h_dist < current_node->h_dist)
				{
					current_node = temp_node;
					current_node_index = static_cast<int>(i);
				}
			}
		}
		map.addSearched(current_node);

		if (current_node->pos == targetNode)
		{
			break;
		}

		// Erase the node from the pool.
		open_nodes.erase(open_nodes.begin() + current_node_index);

		// Add it to the list of nodes we have evaluated.
		closed_nodes.push_back(current_node);

		// Step 2: Evaluate the neighbors of the current node.
		for (auto* neighbor : map.getNeighbors(current_node->pos, diag))
		{
			// If the target node is a neighbor, it's done.  
			quit = *neighbor == map.at(targetNode);

			// Skip neighbors which have been evaluated.
			if (contatinsPtr(closed_nodes, neighbor)) continue;

			// new_travel_cost = cost_so_far + cost_from_current_to_neighbor
			const int new_g_cost = current_node->g_dist + heuristic(current_node->pos, neighbor->pos);

			// Check if neighbor exists within the pool (not evaluated)
			const bool inOpen = contatinsPtr(open_nodes, neighbor);

			// If node DNE in pool, add
			// If node exists and g_cost is cheaper, update.
			if (new_g_cost < neighbor->g_dist || !inOpen)
			{
				neighbor->updateG(new_g_cost);
				neighbor->updateH(heuristic(neighbor->pos, targetNode));
				neighbor->updateF();

				if (!inOpen) open_nodes.push_back(neighbor);

				// nodeTravledTo = nodeTraveledFrom, we will have a chain of these in the end.
				travel_map[neighbor] = current_node;
			}
			//log(neighbor->pos.x, neighbor->pos.y, neighbor->F_dist, neighbor->G_dist);
		}
	}

	// BACKTRACK
	Cell* next = &map.at(targetNode);

	// Just checking while the next parent node exists.
	while (travel_map.count(next))
	{
		next = travel_map[next];
		next->is_path = true;
		if (*next != map.at(startNode))
		{
			travel_path.push_back(next);
		}
	}
	return travel_path;
}

//// MAZE ALGO ////
class MazeAlgo
{
protected:
	int width, height;

	MazeAlgo(const int width,
	         const int height)
		: width(width)
		, height(height)
	{
	}

public:
	virtual                           ~MazeAlgo() = default;
	virtual std::vector<sf::Vector2i> generate() = 0;
};

//// PRIMS ////
inline int getNeighbors(const std::vector<std::vector<int>>& map,
                        const int                            x,
                        const int                            y,
                        const int                            width,
                        const int                            height)
{
	// Gets the neighbors of a given cell, if a given neighbor cell is empty, count goes up by one. (Not a wall)
	int count = 0;
	if (x <= 0 || x >= width - 1 || y <= 0 || y >= height - 1) return 0; // Make sure it isn't out of bounds.
	if (map.at(y + 1).at(x) == 0) count++;
	if (map.at(y - 1).at(x) == 0) count++;
	if (map.at(y).at(x + 1) == 0) count++;
	if (map.at(y).at(x - 1) == 0) count++;
	return count;
}


int clamp(const int x,
          const int min,
          const int max)
{
	if (x > max) return max;
	if (x < min) return min;
	return x;
}


class Prims final :
	public MazeAlgo
{
public:
	using IntVec = std::vector<int>;

	Prims(const int x,
	      const int y) : MazeAlgo(x, y)
	{
	}

	std::vector<sf::Vector2i> generate() override
	{
		const int& height = this->height;
		const int& width = this->width;
		struct Pos
		{
			int x, y;
		};
		std::vector<sf::Vector2i> returnPath{};
		std::vector<IntVec>       map(height, IntVec(width, 1));

		// Choose a random point
		srand(static_cast<int>(time(nullptr)));
		int x = rand() % width;
		srand(static_cast<int>(time(nullptr)));
		int y = rand() % height;
		x = clamp(x, 1, width);
		y = clamp(y, 1, height);

		map.at(y).at(x) = 0;

		// Create a vector containing the points of all neighboring cells (addresses)
		std::vector<Pos> walls;
		walls.push_back(Pos{ x + 1, y });
		walls.push_back(Pos{ x - 1, y });
		walls.push_back(Pos{ x, y + 1 });
		walls.push_back(Pos{ x, y - 1 });
		returnPath.emplace_back(x, y);

		while (!walls.empty())
		{
			srand(static_cast<int>(time(nullptr)));
			const int curWall = rand() % walls.size(); // Get access to a random address and this becomes Current Wall

			x = walls.at(curWall).x;
			// Re-assign x and y, pulling them from the address. (This is why the class Map (vector2i) was created)
			y = walls.at(curWall).y;
			walls.erase(walls.begin() + curWall); // Remove address of the current wall.

			if (getNeighbors(map, x, y, width, height) == 1)
			{
				// If current cell has only one alive neighbor, a passage is possible.
				map.at(y).at(x) = 0; // Make the passage.
				returnPath.emplace_back(x, y);
				walls.push_back(Pos{ x + 1, y });
				walls.push_back(Pos{ x - 1, y });
				walls.push_back(Pos{ x, y + 1 });
				walls.push_back(Pos{ x, y - 1 });
			}
		}
		return returnPath;
	}
};

int main()
{
	Grid               map{ MAP_WIDTH, MAP_HEIGHT };
	sf::RenderWindow   window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Path Finding");
	sf::Vector2i       mousePosition;
	std::vector<Cell*> path{};

	bool               diag = true;
	bool               pathUpdate = true;
	bool               pathBlocked = false; // Cannot route
	std::vector<Cell*> enclosedCellRoom{};
	sf::Clock          clock;

	int searchDrawingTime = 0;
	int pathDrawingTime = 0;
	while (window.isOpen())
	{
		int time = clock.restart().asMilliseconds();
		if (searchDrawingTime / SEARCHED_DISPLAY_RATE < static_cast<int>(map.getSearched().size()))
		{
			searchDrawingTime += time;
		}
		else pathDrawingTime += time;

		float xRatio = static_cast<float>(window.getSize().x) / WINDOW_WIDTH;
		float yRatio = static_cast<float>(window.getSize().y) / WINDOW_HEIGHT;
		mousePosition = sf::Mouse::getPosition(window);

		mousePosition.x -= static_cast<int>(static_cast<float>(window.getSize().x) / 2 - MAP_WIDTH /
			2.f * DEFAULT_TILE_SIZE * xRatio - DEFAULT_TILE_SIZE * xRatio);
		mousePosition.y -= static_cast<int>(static_cast<float>(window.getSize().y) / 2 - MAP_HEIGHT
			/ 2.f * DEFAULT_TILE_SIZE * yRatio - DEFAULT_TILE_SIZE * xRatio);

		sf::Vector2i mouseGridPos{
			mousePosition.x / static_cast<int>((DEFAULT_TILE_SIZE * xRatio)),
			mousePosition.y / static_cast<int>((DEFAULT_TILE_SIZE * yRatio))
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
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::V))
				{
					map.resetGrid();
					map.draw(window);
					Prims prims(MAP_WIDTH + 2, MAP_HEIGHT + 2);
					auto  maze = prims.generate();
					map.read(maze);
					map.draw(window, maze, sf::Color::White,
					         sf::RectangleShape(sf::Vector2f(DEFAULT_TILE_SIZE, DEFAULT_TILE_SIZE)), 0);
				}
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			window.close();
		}

		// Updating and editting positions
		if (
			(sf::Keyboard::isKeyPressed(sf::Keyboard::A)
				|| sf::Keyboard::isKeyPressed(sf::Keyboard::B)
				|| sf::Mouse::isButtonPressed(sf::Mouse::Left)
				|| sf::Mouse::isButtonPressed(sf::Mouse::Right))
			&& Grid::inBounds(mouseGridPos.x, MAP_WIDTH + 1, mouseGridPos.y, MAP_HEIGHT + 1)
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
					pathUpdate = true;
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
					bool  containsA = contatinsPtr(enclosedCellRoom, A);
					bool  containsB = contatinsPtr(enclosedCellRoom, B);
					pathBlocked = !(containsA == containsB);
				}
			}
		}

		// UPDATE SCENE
		if (map.canRoute() && pathUpdate && !pathBlocked)
		{
			searchDrawingTime = 0;
			pathDrawingTime = 0;
			enclosedCellRoom = {};

			// Run A* and get path
			path = aStar(map, map.getA(), map.getB(), diag);

			// If no path can be made, it is blocked.
			pathBlocked = path.empty();

			// Get info about what's in the smallest room (whats being blocked)
			if (pathBlocked)
			{
				auto [_enclosedCell, _target, _enclosedCellRoom] = indiscriminateSearch(
					map, map.getA(), 'B', map.getB(), 'A', diag);

				for (auto var : _enclosedCellRoom)
				{
					enclosedCellRoom.push_back(var);
				}
			}
			pathUpdate = false;
		}

		if (!map.canRoute() || pathBlocked)
		{
			path = {};
			map.resetSearched();
		}
		// CLEAR SCENE
		window.clear();

		// DRAWING
		map.draw(window);

		map.draw(window,
		         enclosedCellRoom,
		         200, 200, 200, 100,
		         sf::CircleShape(DEFAULT_TILE_SIZE / 2.f),
		         static_cast<int>(enclosedCellRoom.size()));

		map.draw(window,
		         map.getSearched(),
		         220, 10, 230, 100,
		         sf::RectangleShape({ DEFAULT_TILE_SIZE, DEFAULT_TILE_SIZE }),
		         searchDrawingTime / SEARCHED_DISPLAY_RATE,
		         0.86f);

		Grid::draw(window,
		           path,
		           sf::Color::Yellow,
		           sf::RectangleShape({ DEFAULT_TILE_SIZE, DEFAULT_TILE_SIZE }),
		           pathDrawingTime / PATH_CONSTRUCTION_RATE,
		           0.42f);

		// DISPLAY
		window.display();
	}
}
