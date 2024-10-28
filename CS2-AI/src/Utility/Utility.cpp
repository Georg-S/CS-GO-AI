#include "Utility/Utility.h"

template <typename T>
inline static std::string to_string_with_max_precision(const T& a_value)
{
	std::stringstream out;
	const int maxPrecision = std::numeric_limits<T>::max_digits10;

	out.precision(maxPrecision);
	out << a_value;
	return out.str();
}

long long get_current_time_in_ms()
{
	auto current_time = std::chrono::system_clock::now();
	return std::chrono::time_point_cast<std::chrono::milliseconds>(current_time).time_since_epoch().count();
}
