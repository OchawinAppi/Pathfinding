//#pragma once
//#include <iostream>
//enum class Direction {
//	NONE,
//	UP,
//	DOWN,
//	LEFT,
//	RIGHT,
//	TOP_LEFT,
//	TOP_RIGHT,
//	BOTTOM_LEFT,
//	BOTTOM_RIGHT
//};
//
//// Overload ostream op ( this is mostly for debugging )
//
//
//static std::ostream& operator<<(std::ostream& os, const Direction& Direction) {
//	switch (Direction)
//	{
//		case Direction::DOWN: 
//		{
//			os << "UP";
//			break;
//		}
//		case Direction::UP: 
//		{
//			os << "DOWN";
//			break;
//		}
//		case Direction::LEFT: 
//		{
//			os << "RIGHT";
//			break;
//		}
//		case Direction::RIGHT: 
//		{
//			os << "LEFT";
//			break;
//		}
//		case Direction::TOP_LEFT: 
//		{
//			os << "TOP_LEFT";
//			break;
//		}
//		case Direction::TOP_RIGHT: 
//		{
//			os << "TOP_RIGHT";
//			break;
//		}
//		case Direction::BOTTOM_LEFT: 
//		{
//			os << "BOTTOM_LEFT";
//			break;
//		}
//		case Direction::BOTTOM_RIGHT: 
//		{
//			os << "BOTTOM_RIGHT";
//			break;
//		}
//		default: os << "NONE";
//	}
//	return os;
//}