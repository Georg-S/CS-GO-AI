#pragma once
#include <chrono>
#include <string>

template <typename T>
static std::string to_string_with_max_precision(const T& a_value);
long long get_current_time_in_ms();
