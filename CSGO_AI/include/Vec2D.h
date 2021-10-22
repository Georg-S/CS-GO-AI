#pragma once
#include <string>

template <typename T>
class Vec2D
{
public:
	Vec2D() : x(0), y(0) {};
	Vec2D(T x, T y) : x(x), y(y) {};

	T x;
	T y;

	void to_string() 
	{
		return "X: " + std::to_string(x) + " Y: " + std::to_string(y);
	}
};