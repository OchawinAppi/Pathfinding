#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>

//// CONFIG ////
constexpr int WINDOW_WIDTH = 800;			// Window stuff
constexpr int WINDOW_HEIGHT = 800;
constexpr int MAP_WIDTH = 40;				// Map stuff
constexpr int MAP_HEIGHT = 40;
constexpr float SIZE_MULTIPLIER = 1.f;		// Tile
constexpr int DECREASE_BOX_SIZE = 5;
constexpr float DEFAULT_TILE_SIZE = static_cast<float>(WINDOW_WIDTH) / (MAP_WIDTH + 2 + DECREASE_BOX_SIZE) * SIZE_MULTIPLIER;
constexpr int SEARCHED_DISPLAY_RATE = 1;	// How fast nodes are displayed ( Higher value == slower )
constexpr int PATH_CONSTRUCTION_RATE = 15;

//// Utils ////
inline sf::Vector2i vfToVi(const sf::Vector2f& vec) {return sf::Vector2i(static_cast<int>(vec.x), static_cast<int>(vec.y));}
inline sf::Vector2f viToVf(const sf::Vector2i& vec) {return sf::Vector2f(static_cast<float>(vec.x), static_cast<float>(vec.y));}
bool isAnyKeyPressed() {
	for (int k = -1; k < sf::Keyboard::KeyCount; ++k) {
		if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(k))) {
			return true;
		}
	}
	return false;
}
sf::Vector2f offset(sf::Vector2f& pos) {
	return sf::Vector2f(
		(pos.x * DEFAULT_TILE_SIZE) + (static_cast<float>(WINDOW_WIDTH) / 2) - (MAP_WIDTH / 2.f * DEFAULT_TILE_SIZE) - DEFAULT_TILE_SIZE,
		(pos.y * DEFAULT_TILE_SIZE) + (static_cast<float>(WINDOW_HEIGHT) / 2) - (MAP_HEIGHT / 2.f * DEFAULT_TILE_SIZE) - DEFAULT_TILE_SIZE);
}

//// CELL /////
class Cell {
public:
	int G_dist;
	int H_dist;
	int F_dist;
	bool isPath = false;
	bool solid;
	sf::Vector2f pos;
	sf::RectangleShape tile;
	char c; 
	Cell() {
		G_dist, H_dist, F_dist = -1;
		pos = sf::Vector2f(-1, -1);
		c = ' ';
		solid = false;
		tile = sf::RectangleShape{};
	}
	Cell(int g, int h, bool _solid, int _x, int _y) :
		G_dist(g), H_dist(h), solid(_solid) {
		F_dist = G_dist + H_dist;
		c = (solid) ? '#' : ' ';
		pos = sf::Vector2f(static_cast<float>(_x), static_cast<float>(_y));
		tile = sf::RectangleShape(sf::Vector2f(DEFAULT_TILE_SIZE, DEFAULT_TILE_SIZE));
		tile.setPosition(offset(pos));
	}
	void makeSolid() { this->solid = true; this->c = '#'; }
	void makeEmpty(){ this->solid = false; this->c = ' '; }
	void updateG(int n) { this->G_dist = n; }
	void updateH(int n) { this->H_dist = n; }
	void updateF() { this->F_dist = this->H_dist + this->G_dist; }
	void setColor(sf::Color color) { this->tile.setFillColor(color); }
	void draw(sf::RenderWindow& window) {
		if (solid) { tile.setFillColor(sf::Color::White); }
		else {
			switch (this->c) {
				case 'A': tile.setFillColor(sf::Color::Red); break;
				case 'B': tile.setFillColor(sf::Color::Blue); break;
				default: tile.setFillColor(sf::Color::Black);
			}
		}
		window.draw(tile);
	}
	bool operator==(const Cell& other) const{ return this->pos.x == other.pos.x && this->pos.y == other.pos.y;}
	bool operator!=(const Cell& other) const { return this->pos.x != other.pos.x || this->pos.y != other.pos.y;}
};

//// GRID ////
class Grid
{
	using CellPtr = std::unique_ptr<Cell>;
	using CellPtrVec = std::vector<CellPtr>;
private:
	std::unique_ptr<std::vector<CellPtrVec>> grid;
	std::vector<Cell*> searched;
	sf::Vector2f aPos;
	sf::Vector2f bPos;
public:
	// Constructor
	Grid(int x, int y) {
		grid = std::unique_ptr<std::vector<CellPtrVec>>(new std::vector<CellPtrVec>());
		x += 2;
		y += 2;
		for (int i = 0; i < static_cast<int>(y); ++i) {
			grid->push_back({});
			for (int j = 0; j < static_cast<int>(x); ++j) {
				if (i == 0 || i == y - 1 || j == 0 || j == x - 1) {
					grid->at(i).push_back(std::make_unique<Cell>(0, 0, true, j, i));
				}
				else {
					grid->at(i).push_back(std::make_unique<Cell>(0, 0, false, j, i));
				}
			}
		}
		grid = std::move(grid);
		aPos = sf::Vector2f(-1, -1);
		bPos = sf::Vector2f(-1, -1);
	}
	virtual ~Grid() = default;
	void draw(sf::RenderWindow& window) {
		for (int y = 0; y < static_cast<int>(grid->size()); y++) {
			for (int x = 0; x < static_cast<int>(grid->at(0).size()); x++) {
				grid->at(y).at(x)->draw(window);
			}
		}
	}
	void draw(sf::RenderWindow& window, std::vector<Cell*> path, sf::Color color, sf::Shape&& shape, int drawCount, float scale) {
		sf::Vector2f newScale(scale, scale);
		float delta = (static_cast<float>(DEFAULT_TILE_SIZE) - DEFAULT_TILE_SIZE * scale) / 2;
		shape.setFillColor(color);
		int count = 0;
		for (const auto& tile : path) {
			if (drawCount == count) return;
			if (tile->c != ' ' || tile->solid) continue;
			shape.setScale(newScale);
			shape.setPosition(offset(tile->pos));
			shape.move(sf::Vector2f(delta, delta));
			window.draw(shape);
			++count;
		}
	}
	void draw(sf::RenderWindow& window, std::vector<Cell*> path, int r, int g, int b, int a, sf::Shape&& shape, int drawCount, float scale) {
		draw(window, path, sf::Color(r, g, b, a), std::forward<sf::Shape>(shape), drawCount, scale);
	}
	inline Cell& at(int x, int y) { return *grid->at(y).at(x); }
	inline Cell& at(const sf::Vector2i& pos) { return at(pos.x, pos.y); }
	inline Cell& at(const sf::Vector2f& pos) { return at(static_cast<int>(pos.x), static_cast<int>(pos.y));}
	inline bool inBounds(int x, int x_size, int y, int y_size) { return (x > 0 && x < x_size) && (y > 0 && y < y_size);}
	inline void initA(const sf::Vector2f& pos) { aPos = sf::Vector2f(pos); at(pos).c = 'A'; }
	void initB(const sf::Vector2f& pos) { bPos = sf::Vector2f(pos); at(pos).c = 'B'; }
	bool moveA(const sf::Vector2f& pos) {
		if (!at(pos).solid && at(pos).c == ' ') {
			if (aPos.x == -1) { initA(pos); return true; }
			at(aPos).makeEmpty();
			aPos.x = pos.x;
			aPos.y = pos.y;
			at(pos).c = 'A';
			return true;
		}
		return false;
	}
	bool moveB(const sf::Vector2f& pos) {
		if (!at(pos).solid && at(pos).c == ' ') {
			if (bPos.x == -1) { initB(pos); return true; }
			at(bPos).makeEmpty();
			bPos.x = pos.x;
			bPos.y = pos.y;
			at(pos).c = 'B';
			return true;
		}
		return false;
	}
	bool canRoute() { return this->getA().x != -1 && this->getB().x != -1; }
	void resetA() { aPos.x = -1; aPos.y = -1; }
	void resetB() { bPos.x = -1; bPos.y = -1; }
	void resetPath() {
		for (int y = 1; y < MAP_HEIGHT + 1; ++y) {
			for (int x = 1; x < MAP_WIDTH + 1; ++x) {
				this->at(x, y).isPath = false;
			}
		}
	}
	void resetGrid() {
		resetA();
		resetB();
		for (int y = 1; y < MAP_HEIGHT + 1; ++y) {
			for (int x = 1; x < MAP_WIDTH + 1; ++x) {
				this->at(x, y).F_dist = 0;
				this->at(x, y).G_dist = 0;
				this->at(x, y).H_dist = 0;
				this->at(x, y).solid = false;
				this->at(x, y).c = ' ';
				this->at(x, y).isPath = false;
			}
		}
	}
	std::vector<Cell*> getNeighbors(const sf::Vector2f& pos, bool diag) {
		int x{ static_cast<int>(pos.x) };
		int y{ static_cast<int>(pos.y) };
		std::vector<Cell*> neighbors{};
		if (diag) {
			for (int _x = x - 1; _x < x + 2; ++_x) {
				if (!(_x == x) && this->inBounds(_x, MAP_WIDTH + 1, y, MAP_HEIGHT + 1)) {
					if (Cell* cell = &at(_x, y); !cell->solid) {
						neighbors.push_back(cell);
					}
				}
			}
			for (int _y = y - 1; _y < y + 2; ++_y) {
				if (!(_y == y) && this->inBounds(x, MAP_WIDTH + 1, _y, MAP_HEIGHT + 1)) {
					if (Cell* cell = &at(x, _y); !cell->solid) {
						neighbors.push_back(cell);
					}
				}
			}
			return neighbors;
		}
		for (int _x = x - 1; _x < x + 2; _x++) {
			for (int _y = y - 1; _y < y + 2; _y++) {
				if (!(_x == x && _y == y) && this->inBounds(_x, MAP_WIDTH + 1, _y, MAP_HEIGHT + 1)) {
					if (Cell* cell = &at(_x, _y); !cell->solid) {
						neighbors.push_back(cell);
					}
				}
			}
		}
		return neighbors;
	}
	sf::Vector2f& getB() { return this->bPos; }
	sf::Vector2f& getA() { return this->aPos; }
	std::vector<Cell*>& getSearched() { return searched; }
	void addSearched(Cell* cell) { searched.push_back(cell); }
	void resetSearched() { searched.clear(); resetPath(); }
};

inline bool contatinsPtr(const std::vector<Cell*>& vec, Cell*& item) {
	for (int i = 0; i < static_cast<int>(vec.size()); i++) { if (vec.at(i) == item) { return true; } }
	return false;
}

template <typename T>
inline auto contains(const std::vector<T>& vec, const T& _item) {
	struct returnInfo { bool contains; int index; };
	for (int i = 0; i < static_cast<int>(vec.size()); i++) { if (vec.at(i) == _item) return returnInfo{ true, i }; }
	return returnInfo{ false , -1 };
}

//// FIND IF TARGET IS REACHABLE ////
auto indiscriminateSearch(Grid& map, sf::Vector2f& start_node1, char target_letter1, sf::Vector2f& start_node2, char target_letter2, bool diag)
{
	struct returnInfo { Cell* node; char target; std::vector<Cell*> room; };
	std::vector<Cell*> collectedNodes1{ &map.at(start_node1) };
	std::vector<Cell*> collectedNodes2{ &map.at(start_node2) };
	Cell* current_node1 = nullptr;
	Cell* current_node2 = nullptr;
	int count = 0;
	while (static_cast<int>(collectedNodes1.size()) > count && collectedNodes2.size() > count) {
		current_node1 = collectedNodes1.at(count);
		current_node2 = collectedNodes2.at(count);
		if (current_node1->c == target_letter1) return returnInfo{ &map.at(start_node1), target_letter1, collectedNodes1 };
		if (current_node2->c == target_letter2) return returnInfo{ &map.at(start_node2), target_letter2, collectedNodes2 };
		const auto& neighbors1 = map.getNeighbors(current_node1->pos, diag);
		if (neighbors1.size() == 0) returnInfo{ &map.at(start_node1), target_letter1, collectedNodes1 };
		const auto& neighbors2 = map.getNeighbors(current_node2->pos, diag);
		if (neighbors2.size() == 0) returnInfo{ &map.at(start_node2), target_letter2, collectedNodes2 };
		for (auto* neighbor : neighbors1) {
			if (contatinsPtr(collectedNodes1, neighbor)) continue;
			collectedNodes1.push_back(neighbor);
		}
		for (auto* neighbor : neighbors2) {
			if (contatinsPtr(collectedNodes2, neighbor)) continue;
			collectedNodes2.push_back(neighbor);
		}
		++count;
	}
	if (collectedNodes1.size() <= count) return returnInfo{ &map.at(start_node1), target_letter1, collectedNodes1 };
	else return returnInfo{ &map.at(start_node2), target_letter2, collectedNodes2 };
}

//// GET DISTANCE ////
inline int getDistance(sf::Vector2f& node, sf::Vector2f& target) {
	int x_dist = static_cast<int>(abs(node.x - target.x));
	int y_dist = static_cast<int>(abs(node.y - target.y));
	int distance = 0;
	while (x_dist >= 1 && y_dist >= 1) {
		x_dist--;
		y_dist--;
		distance += 14;
	}
	distance += x_dist * 10;
	distance += y_dist * 10;
	return distance;
}

//// A STAR ////
std::vector<Cell*> a_star(Grid& map, sf::Vector2f& start_node, sf::Vector2f& target_node, bool diag) {
	map.resetSearched();
	map.at(start_node).updateG(0);
	map.at(start_node).updateH(getDistance(start_node, target_node));
	map.at(start_node).updateF();
	std::vector<Cell*> open_nodes{ &map.at(start_node) };
	std::vector<Cell*> closed_nodes{};
	std::unordered_map<Cell*, Cell*> travelMap;
	std::vector<Cell*> travelPath{};
	Cell* current_node = nullptr;
	bool quit = false;
	while (open_nodes.size() > 0 && !quit) {
		current_node = open_nodes.at(0);
		int current_node_index = 0;
		for (size_t i = 1; i < open_nodes.size(); ++i) {
			auto* temp_node = open_nodes.at(i);
			if (temp_node->F_dist < current_node->F_dist) {
				current_node = temp_node;
				current_node_index = i;
			}
			else if (temp_node->F_dist == current_node->F_dist) {
				if (temp_node->H_dist < current_node->H_dist) {
					current_node = temp_node;
					current_node_index = i;
				}
			}
		}
		map.addSearched(current_node);
		if ((current_node->pos.x == target_node.x) && (current_node->pos.y == target_node.y)){break;}
		open_nodes.erase(open_nodes.begin() + current_node_index);
		closed_nodes.push_back(current_node);
		for (auto* neighbor : map.getNeighbors(current_node->pos, diag)) {
			quit = (*neighbor == map.at(target_node));
			if (contatinsPtr(closed_nodes, neighbor)) continue;
			const int new_g_cost = current_node->G_dist + getDistance(current_node->pos, neighbor->pos);
			bool inOpen = contatinsPtr(open_nodes, neighbor);
			if (new_g_cost < neighbor->G_dist || !inOpen) {
				neighbor->updateG(new_g_cost);
				neighbor->updateH(getDistance(neighbor->pos, target_node));
				neighbor->updateF();
				if (!inOpen) open_nodes.push_back(neighbor);
				travelMap[neighbor] = current_node;
			}
		}
	}
	Cell* next = &map.at(target_node);
	while (travelMap.count(next)) {
		next = travelMap[next];
		next->isPath = true;
		if (*next != map.at(start_node)) { travelPath.push_back(next); }
	}
	return travelPath;
}


int main() {
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Path Finding");
	Grid map{ MAP_WIDTH, MAP_HEIGHT };
	sf::Vector2i mousePosition;
	std::vector<Cell*> path{};
	bool diag = true;
	bool pathUpdate = true;
	bool pathBlocked = false; // Cannot route	
	sf::Vector2f* enclosedCell{};
	std::vector<Cell*> enclosedCellRoom{};
	char target{};
	sf::Clock clock;
	int searchDrawingTime = 0;
	int pathDrawingTime = 0;
	while (window.isOpen())
	{
		int time = clock.restart().asMilliseconds();
		if (searchDrawingTime / SEARCHED_DISPLAY_RATE < static_cast<int>(map.getSearched().size())) {
			searchDrawingTime += static_cast<int>(time);
		}
		else pathDrawingTime += static_cast<int>(time);
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
			if (event.type == sf::Event::Closed) { window.close(); }
			if (event.type == sf::Event::KeyPressed) {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
					diag = !diag;
					pathUpdate = true;
					pathBlocked = false;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
					map.resetGrid();
					enclosedCellRoom.clear();
				}
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) { window.close(); }
		if (
			(sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::B) || sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Mouse::isButtonPressed(sf::Mouse::Right)) &&
			(map.inBounds(mouseGridPos.x, MAP_WIDTH + 1, mouseGridPos.y, MAP_HEIGHT + 1))
			)
		{
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
				if (char c = map.at(mouseGridPos).c; c != ' ') {
					if (c == 'A') { map.resetA(); }
					else if (c == 'B') { map.resetB(); }
					map.at(mouseGridPos).makeEmpty();
					pathBlocked = false;
					pathUpdate = true;
				}
			}
			if (!map.at(mouseGridPos).solid) {
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					if (char c = map.at(mouseGridPos).c; c != ' ') {
						if (c == 'A') { map.resetA(); }
						else if (c == 'B') { map.resetB(); }
					}
					map.at(mouseGridPos).makeSolid();
					pathUpdate = true;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { pathUpdate = map.moveA(viToVf(mouseGridPos)); }
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::B)) { pathUpdate = map.moveB(viToVf(mouseGridPos)); }
				if (pathBlocked && map.canRoute()) {
					Cell* A = &map.at(map.getA());
					Cell* B = &map.at(map.getB());
					bool containsA = contatinsPtr(enclosedCellRoom, A);
					bool containsB = contatinsPtr(enclosedCellRoom, B);
					pathBlocked = !(containsA == containsB);
				}
			}
		}
		if (map.canRoute() && pathUpdate && !pathBlocked)
		{
			searchDrawingTime = 0;
			pathDrawingTime = 0;
			enclosedCellRoom = {};
			path = a_star(map, map.getA(), map.getB(), diag);
			pathBlocked = (path.empty());
			if (pathBlocked) {
				auto [_enclosedCell, _target, _enclosedCellRoom] = indiscriminateSearch(map, map.getA(), 'B', map.getB(), 'A', diag);
				enclosedCell = nullptr;
				enclosedCell = &_enclosedCell->pos;
				target = _target;
				for (auto var : _enclosedCellRoom) {enclosedCellRoom.push_back(var); }
			}
			pathUpdate = false;
		}
		if (!map.canRoute() || pathBlocked) { path = {}; map.resetSearched(); }
		window.clear();
		map.draw(window);
		map.draw(window, enclosedCellRoom, 200, 200, 200, 100, sf::CircleShape(DEFAULT_TILE_SIZE / 2.f), enclosedCellRoom.size(), 1);
		map.draw(window, map.getSearched(), 200, 200, 200, 100, sf::RectangleShape(sf::Vector2f(DEFAULT_TILE_SIZE, DEFAULT_TILE_SIZE)), searchDrawingTime / SEARCHED_DISPLAY_RATE, 0.86f);
		map.draw(window, path, sf::Color::Yellow, sf::RectangleShape(sf::Vector2f(DEFAULT_TILE_SIZE, DEFAULT_TILE_SIZE)), pathDrawingTime / PATH_CONSTRUCTION_RATE, 0.42f);
		window.display();
	}
}