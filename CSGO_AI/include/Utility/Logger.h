#pragma once
#include <string>

class Logger 
{
public:
	virtual ~Logger() = default;
	virtual void log(const std::string& str) = 0;
	virtual void log_error(const std::string& str) = 0;
	virtual void log_success(const std::string& str) = 0;
};