#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <string>

constexpr float PI = static_cast<float>(M_PI);

template <typename T>
struct Vec3D
{
	Vec3D() : x(0), y(0), z(0) {}
	Vec3D(T x, T y, T z) : x(x), y(y), z(z)  {}


	T calc_abs() const
	{
		return sqrt(x * x + y * y + z * z);
	}

	void normalize()
	{
		T length = calc_abs();
		x /= length;
		y /= length;
		z /= length;
	}

	T dot_product(const Vec3D<T>& other) const
	{	
		return x * other.x + y * other.y + z * other.z;
	}

	T distance(const Vec3D<T>& other) const
	{
		return (*this - other).calc_abs();
	}

	std::string to_string() const
	{
		return "X: " + std::to_string(x) + " Y: " + std::to_string(y) + " Z: " + std::to_string(z);
	}

	T x;
	T y;
	T z;
};

template <typename T>
const Vec3D<T> operator+(const Vec3D<T>& left, const Vec3D<T>& right)
{
	Vec3D<T> result;
	result.x = left.x + right.x;
	result.y = left.y + right.y;
	result.z = left.z + right.z;

	return result;
}

template <typename T>
const Vec3D<T> operator-(const Vec3D<T>& left, const Vec3D<T>& right)
{
	Vec3D<T> result;
	result.x = left.x - right.x;
	result.y = left.y - right.y;
	result.z = left.z - right.z;

	return result;
}

template <typename T, typename R>
const Vec3D<T> operator/(const Vec3D<T>& left, R num)
{
	Vec3D<T> result;
	result.x = left.x / num;
	result.y = left.y / num;
	result.z = left.z / num;

	return result;
}

template <typename T, typename R>
const Vec3D<T> operator*(const Vec3D<T>& left, T num)
{
	Vec3D<T> result;
	result.x = left.x * num;
	result.y = left.y * num;
	result.z = left.z * num;

	return result;
}