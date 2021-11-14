#include "Utility/Logging.h"

std::unique_ptr<Logger> Logging::logger = nullptr;
bool Logging::valid_logger_set = false;


void Logging::log_error(const std::string& str)
{
	if (Logging::valid_logger_set)
		logger->log_error(str);
}

void Logging::log_success(const std::string& str)
{
	if (Logging::valid_logger_set)
		logger->log_success(str);
}

void Logging::log(const std::string& str)
{
	if (Logging::valid_logger_set)
		logger->log(str);
}

void Logging::set_logger(std::unique_ptr<Logger> logger)
{
	Logging::valid_logger_set = logger ? true : false;
	Logging::logger = std::move(logger);
}

void Logging::invalidate_logger()
{
	Logging::valid_logger_set = false;
}
