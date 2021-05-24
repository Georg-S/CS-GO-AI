#pragma once
#include <fstream>
#include <string>
#include <iostream>

class FileReader
{
public:
	static std::string read_value_of_string_in_file(const std::string& fileName, const std::string& key);
	static std::string get_string_between_quotationmarks(const std::string& input);
};

