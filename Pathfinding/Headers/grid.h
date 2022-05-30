#pragma once
#include "cell.h"


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
	     int y);
	// Destructor not needed. The compiler got this. ;) ( probably )

	// Displaying - Console
	void                 print() const;
	friend std::ostream& operator<<(std::ostream& os,
	                                const Grid&   grid);

	// Displaying - SFML
	void        draw(sf::RenderWindow& window) const;
	static void draw(sf::RenderWindow&         window,
	                 const std::vector<Cell*>& path,
	                 sf::Color                 color,
	                 sf::Shape&&               shape,
	                 int                       drawCount,
	                 float                     scale = 1.f);
	void draw(sf::RenderWindow&         window,
	          const std::vector<Cell*>& path,
	          sf::Uint8                 r,
	          sf::Uint8                 g,
	          sf::Uint8                 b,
	          sf::Uint8                 a,
	          sf::Shape&&               shape,
	          int                       drawCount,
	          float                     scale = 1.f) const;
	void draw(sf::RenderWindow&                window,
	          const std::vector<sf::Vector2i>& path,
	          sf::Color                        color,
	          sf::Shape&&                      shape,
	          int                              drawCount,
	          float                            scale = 1.f) const;
	void draw(sf::RenderWindow&                window,
	          const std::vector<sf::Vector2i>& path,
	          sf::Uint8                        r,
	          sf::Uint8                        g,
	          sf::Uint8                        b,
	          sf::Uint8                        a,
	          sf::Shape&&                      shape,
	          int                              drawCount,
	          float                            scale = 1.f) const;


	// Getting Specific cell ( will need this later )
	[[nodiscard]] inline Cell& at(int x,
	                              int y) const;
	[[nodiscard]] Cell& at(const sf::Vector2f& pos) const;
	[[nodiscard]] Cell& at(const sf::Vector2i& pos) const;

	static bool inBounds(int x,
	                     int x_size,
	                     int y,
	                     int y_size);

	// Points 
	void initA(const sf::Vector2f& pos);
	void initB(const sf::Vector2f& pos);
	bool moveA(const sf::Vector2f& pos);
	bool moveB(const sf::Vector2f& pos);
	bool canRoute();
	void resetA();
	void resetB();
	void resetGrid();
	void resetPath() const;

	void read(const std::vector<sf::Vector2i>& mapInfo) const;

	[[nodiscard]] std::vector<Cell*> getNeighbors(const sf::Vector2f& pos,
	                                              bool                diag) const;
	sf::Vector2f& getB();
	sf::Vector2f& getA();

	static inline sf::Vector2f offsetPosition(const sf::Vector2f& position);

	std::vector<Cell*>& getSearched();
	void                addSearched(Cell* cell);
	void                resetSearched();
};
