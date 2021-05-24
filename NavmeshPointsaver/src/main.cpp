#pragma once
#include <iostream>
#include <fstream>
#include "NavmeshPoints.h"
#include "json.hpp"

using json = nlohmann::json;

int main()
{
	/*
	std::ifstream ifs("offsets.json");
	json jf = json::parse(ifs);
	ifs.close();
	std::cout << jf["netvars"]["m_ArmorValue"] << std::endl;
	*/

	NavmeshPoints program = NavmeshPoints();

	if (program.init())
		program.run();

	return 0;
}
