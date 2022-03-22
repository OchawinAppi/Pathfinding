# Pre-requisites

You need to have SFML installed.\
Ubuntu -> `sudo apt-get install libsfml-dev`\
MacOs -> `brew install sfml`\
Others -> https://www.sfml-dev.org/download/sfml/2.5.1/

Note: For window users you could grab the working application from release without downloading anything else.\
However, if you wish to build the project on your machine then you might have to install SFML.\
If you wish to build on windows via g++, you need to install `SFML for GCC 7.3.0 MinGW`.\
Otherwise, if you use MSC++ to build, then there is no need to install SFML.

# Build Instructions ( CLI )

For `Linux`/ `Mac`
1. `wget https://raw.githubusercontent.com/OchawinAppi/Pathfinding/master/Pathfinding/build.cpp`
2. `g++ build.cpp -O3 -o pathfinder -std=c++17 -lsfml-window -lsfml-system -lsfml-graphics`
3. `./pathfinder`

For `windows g++` (Replace <PATH_TO_SFML_SFML-2.5.1> on line 3)
1. `wget https://raw.githubusercontent.com/OchawinAppi/Pathfinding/master/Pathfinding/build.cpp`
2. `SET "sfml_path=<PATH_TO_SFML_SFML-2.5.1>"` eg. SET "sfml_path=C:\Programs\SFML-2.5.1"
3. `xcopy "%sfml_path%"\bin\*.dll .`
4. `g++ -I"%sfml_path%"\include -L"%sfml_path%"\lib build.cpp -O3 -o pathfinder -std=c++17 -lsfml-window -lsfml-system -lsfml-graphics`
5. `pathfinder.exe`

Configurations can be made by editting the variables declared in CONFIG in the code.

## Controls:
`A` - Place Start Node\
`B` - Place End Node\
`LC` - Place Wall\
`LR` - Remove wall\
`C` - Clear\
`Space` - Toggle Diagonal Movement

# Pathfinding

This is a demonstration of the A* path finding algorithm.
The pathfinder finds the shortest path between two nodes, A and B.

# Method

The algorithim begins by picking the start node as its current node. It then evaluates the neighboring nodes, calculating their values. 
The cost of travelling is calculated by:

F_COST = G_COST + H_COST 

G_COST = the cost so far to current node\
H_COST = the cost to the end node. (Distance from end node)

Once evaluated, the neighbors will be added into a pool. Then the algorithm will pick the node with the lowest F_COST to be its current node. 
And if there are more than one with the same lowest F_COST, it is evaluated by the lowest H_COST.

For this program, the tiles are squares, so we can simply calculate the distance using the pythagorean theorem. 
To avoid sqrt and pow for speed, this is simply calculated by calculating all the diagonals first, then multiplying whatever is left over by 10. 

(Note for simplicity, we use 10 and 14 instead of 1.0 and 1.4 because integers are easier to deal with)


# Example - Simple scenario

STARTNODE: A\
ENDNODE: B

First, we calculate the neighbors of the start node

NOTE: initial cost is 0.

G_COSTs: (Cost so far + distance to neighbor)
|14|10|14|?|?|?|
|---|---|---|---|---|---|
|10|**A**|10|?|?|**B**|
|14|10|14|?|?|?|

H_COSTs
|54|44|34|24|14|10|
|---|---|---|---|---|---|
|50|**A**|30|20|10|**B**|
|54|44|34|24|14|10|


F_COSTs (H_COST + G_COST)
|68|54|48|?|?|?|
|---|---|---|---|---|---|
|60|**A**|40|?|?|**B**|
|68|54|48|?|?|?|

Note: **Only** neighbor cells are evaluated

Algorithim picks the node with the lowest value, 40

G_COSTs
|68|54|48|62|?|?|
|---|---|---|---|---|---|
|60|**A**|X|50|?|**B**|
|68|54|48|62|?|?|


IMPORTANT NOTE:\
2 of the new current node's neighbors exists within the pool already.\
Their F_COST and G_COST will be replaced if their calculated F_COST in the current 
iteration is lower.\
This may not seem very useful in this obstacle-less example, but when there is an obstacle, this will help get the shortest path.

# Backtrack

When a neighbor is evaluated, the current cell becomes the "parent" node of the neighbor cell.\
When a neighbor cell is replaced (Explanation above), the parent node will change to the current node of the iteration. 

By the end we will have a chain of cells leading from end to start, like so.
|🡖|🡓|🡗|🡗|🡗|🡗|
|---|---|---|---|---|---|
🡒|**A**|🡐|🡐|🡐|**B**|
|🡕 |🡑|🡔|🡔|🡔|🡔|

Yeilding the path:
| | | | | |
|---|---|---|---|---|
|**A**|🡐|🡐|🡐|**B**|
| | | | | |

Explanation Video: To be added
