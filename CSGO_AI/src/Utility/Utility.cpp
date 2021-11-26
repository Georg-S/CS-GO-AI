#include "Utility/Utility.h"

long long get_current_time_in_ms()
{
	auto current_time = std::chrono::system_clock::now();
	return std::chrono::time_point_cast<std::chrono::milliseconds>(current_time).time_since_epoch().count();
}
