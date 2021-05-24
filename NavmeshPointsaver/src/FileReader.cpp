#include "FileReader.h"

std::string FileReader::read_value_of_string_in_file(const std::string& fileName, const std::string& key)
{
	std::ifstream file(fileName);
	std::string str, returnString;

	while (std::getline(file, str)) {

		std::size_t found = str.find(key);
		if (found != std::string::npos)
			returnString = FileReader::get_string_between_quotationmarks(str);
	}

	return returnString;
}

std::string FileReader::get_string_between_quotationmarks(const std::string& input)
{
	unsigned first = input.find("\"");
	unsigned last = input.find_last_of("\"");
	return input.substr(first+1, last-first-1);
}
