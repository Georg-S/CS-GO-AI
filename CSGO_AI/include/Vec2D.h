#pragma once
template <typename T>
class Vec2D
{
public:
	Vec2D() : x(0), y(0) {};
	Vec2D(T x, T y) : x(x), y(y) {};

	T x;
	T y;

	void print()
	{
		std::cout << "X: " << x << " Y: " << y << std::endl;
	}
};