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

template <typename T>
Vec2D<T> operator-(const Vec2D<T>& left, const Vec2D<T>& right)
{
	Vec2D<T> result;
	result.x = left.x - right.x;
	result.y = left.y - right.y;

	return result;
}