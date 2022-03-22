#pragma once

// Window stuff
constexpr int WINDOW_WIDTH = 1200;
constexpr int WINDOW_HEIGHT = 800;

// Map stuff
constexpr int MAP_WIDTH = 60;
constexpr int MAP_HEIGHT = 120;

// Tile
//constexpr float DEFAULT_TILE_SIZE = ((float)WINDOW_WIDTH/MAP_WIDTH) - 3.f;
constexpr float SIZE_MULTIPLIER = 1.f;
constexpr float DEFAULT_TILE_SIZE = static_cast<float>(WINDOW_WIDTH)/(MAP_WIDTH+1) * SIZE_MULTIPLIER;

// How fast nodes are displayed ( Higher value == slower )

constexpr int SEARCHED_DISPLAY_RATE = 1;
constexpr int PATH_CONSTRUCTION_RATE = 15;
