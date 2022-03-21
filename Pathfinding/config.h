#pragma once

// Window stuff
constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 800;

// Map stuff
constexpr int MAP_WIDTH = 40;
constexpr int MAP_HEIGHT = 40;

// Tile
//constexpr float DEFAULT_TILE_SIZE = ((float)WINDOW_WIDTH/MAP_WIDTH) - 3.f;
constexpr float DEFAULT_TILE_SIZE = 17.f;

// How fast nodes are displayed ( Higher value == slower )

constexpr int SEARCHED_DISPLAY_RATE = 1;
constexpr int PATH_CONSTRUCTION_RATE = 15;
