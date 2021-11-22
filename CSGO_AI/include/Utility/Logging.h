#pragma once
#include <string>
#include "Logger.h"

class Logging 
{
public:
	static void log_error(const std::string& str);
	static void log_success(const std::string& str);
	static void log(const std::string& str);
	static void set_logger(Logger* logger);
	static void invalidate_logger();

private:
	static Logger* logger;
	static bool valid_logger_set;
};