#pragma once
#include <math.h>

template <typename T>
class Vec3D
{
public:
	Vec3D() : x(0), y(0), z(0) {}
	Vec3D(int x, int y, int z) : x(x), y(y), z(z)  {}


	T calc_abs() const
	{
		return sqrt(x * x + y * y + z * z);
	}

	void print() const
	{
		std::cout << "X: " << x << " Y: " << y << " Z: " << z << std::endl;
	}

	void normalize() 
	{
		T length = calc_abs();
		x /= length;
		y /= length;
		z /= length;
	}

	T dot_product(const Vec3D<T> other) const
	{	
		return x * other.x + y * other.y + z * other.z;
	}

	T x;
	T y;
	T z;
};

template <typename T>
Vec3D<T> operator+(const Vec3D<T>& left, const Vec3D<T>& right)
{
	Vec3D<T> result;
	result.x = left.x + right.x;
	result.y = left.y + right.y;
	result.z = left.z + right.z;

	return result;
}

template <typename T>
Vec3D<T> operator-(const Vec3D<T>& left, const Vec3D<T>& right)
{
	Vec3D<T> result;
	result.x = left.x - right.x;
	result.y = left.y - right.y;
	result.z = left.z - right.z;

	return result;
}

template <typename T>
Vec3D<T> operator/(const Vec3D<T>& left, T num)
{
	Vec3D<T> result;
	result.x /= num;
	result.y /= num;
	result.z /= num;

	return result;
}

template <typename T>
Vec3D<T> operator*(const Vec3D<T>& left, T num)
{
	Vec3D<T> result;
	result.x *= num;
	result.y *= num;
	result.z *= num;

	return result;
}