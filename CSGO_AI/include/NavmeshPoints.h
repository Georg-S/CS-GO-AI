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
#include "Logger.h"

using json = nlohmann::json;

class NavmeshPoints 
{
public:
	NavmeshPoints(std::shared_ptr<Logger> logger, std::shared_ptr<GameInformationhandler> game_info_handler);
	bool init();
	void console_run();
	bool update();
	void add_point();
	void set_add_point_button(int key_code);
	bool save_to_file();
	Vec3D<float> get_latest_point();

private:
	template <typename T>
	static std::string to_string_with_max_precision(const T& a_value);

	Button save_button;
	Button close_button;
	Offsets* offsets;
	ConfigData config;
	MemoryManager mem_manager;
	DWORD client_dll_address;
	DWORD engine_address;
	std::vector<Vec3D<float>> points;
	std::shared_ptr<GameInformationhandler> game_info_handler = nullptr;
	std::shared_ptr<Logger> logger = nullptr;
};
