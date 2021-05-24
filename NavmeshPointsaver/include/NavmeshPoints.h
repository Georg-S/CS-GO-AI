#pragma once
#include <fstream>
#include <sstream>
#include <vector>
#include "Button.h"
#include "Offsets.h"
#include "MemoryManager.h"
#include "ConfigData.h"
#include "Vec3D.h"
#include "json.hpp"

using json = nlohmann::json;

class NavmeshPoints 
{
public:
	NavmeshPoints();
	bool init();
	void run();
	Vec3D<float> get_current_position();
	void put_vec3d_in_vec(const Vec3D<float>& vec);

private:
	bool load_config();
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
};
