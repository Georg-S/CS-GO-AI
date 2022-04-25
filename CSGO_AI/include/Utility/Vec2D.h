#pragma once
#include <string>

template <typename T>
struct Vec2D
{
	Vec2D() : x(0), y(0) {};
	Vec2D(T x, T y) : x(x), y(y) {};

	T x;
	T y;

	T distance(const Vec2D<T>& other) const
	{
		int x_dif = other.x - x;
		int y_dif = other.y - y;

		return sqrt(x_dif * x_dif + y_dif * y_dif);
	}

	std::string to_string() const
	{
		return "X: " + std::to_string(x) + " Y: " + std::to_string(y);
	}
};

template <typename T>
const Vec2D<T> operator-(const Vec2D<T>& left, const Vec2D<T>& right)
{
	Vec2D<T> result;
	result.x = left.x - right.x;
	result.y = left.y - right.y;

	return result;
}