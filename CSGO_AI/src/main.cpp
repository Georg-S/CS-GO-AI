#pragma once
#include <iostream>
#include <fstream>
#include <Offsets.h>
#include "json.hpp"

using json = nlohmann::json;

int main()
{
	if (!Offsets::load_offsets_from_file("offsets.json")) 
	{
		std::cout << "Offsets couldn't be read, make sure you have a valid offets file" << std::endl;
		getchar();
		return 0;
	}


	return 0;
}
