#pragma once
#include <fstream>
#include <sstream>
#include <vector>
#include "Button.h"
#include "Offsets.h"
#include "MemoryManager.h"
#include "Utility/Vec3D.h"
#include "ConfigReader.h"
#include "Utility/json.hpp"
#include "GameInformationHandler.h"
#include "Utility/Logging.h"

using json = nlohmann::json;

class NavmeshPoints 
{
public:
	NavmeshPoints(std::shared_ptr<GameInformationhandler> game_info_handler);
	bool init();
	bool update();
	void add_point();
	void set_add_point_button(int key_code);
	bool save_to_file();
	std::pair<bool, Vec3D<float>> get_current_position() const;

private:
	Button save_button;
	Button close_button;
	Offsets* offsets;
	ConfigData config;
	MemoryManager mem_manager;
	DWORD client_dll_address;
	DWORD engine_address;
	std::string map_name = "";
	std::vector<Vec3D<float>> points;
	std::shared_ptr<GameInformationhandler> game_info_handler = nullptr;
};
