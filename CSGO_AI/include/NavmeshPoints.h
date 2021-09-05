#pragma once
#include <fstream>
#include <sstream>
#include <vector>
#include "Button.h"
#include "Offsets.h"
#include "MemoryManager.h"
#include "Vec3D.h"
#include "ConfigReader.h"
#include "json.hpp"
#include "GameInformationHandler.h"

using json = nlohmann::json;

class NavmeshPoints 
{
public:
	NavmeshPoints();
	bool init();
	void console_run();

private:
	template <typename T>
	static std::string to_string_with_max_precision(const T& a_value);
	void save_to_file();

	Button save_button;
	Button close_button;
	Offsets* offsets;
	ConfigData config;
	MemoryManager mem_manager;
	DWORD client_dll_address;
	DWORD engine_address;
	std::vector<Vec3D<float>> points;
	std::unique_ptr<GameInformationhandler> game_info_handler = nullptr;
};
