#include "NavmeshPoints.h"

NavmeshPoints::NavmeshPoints()
{
	constexpr int l_key = 0x4C;
	this->save_button = Button();
	this->close_button = Button("Close button", l_key);
	mem_manager = MemoryManager();
}

bool NavmeshPoints::init()
{
	if (!load_config())
	{
		std::cout << "Loading config failed, check config file" << std::endl;
		return false;
	}

	if (!Offsets::load_offsets_from_file())
	{
		std::cout << "Loading offsets failed, check offsets file" << std::endl;
		return false;
	}

	if (!mem_manager.attach_to_process(config.windowname.c_str()))
	{
		std::cout << "Couldn't find process: Quiting " << std::endl;
		std::cout << "Make Sure CS-GO is open " << std::endl;
		return false;
	}

	client_dll_address = mem_manager.get_module_address(config.client_dll_name.c_str());
	engine_address = mem_manager.get_module_address(config.engine_dll_name.c_str());
	return client_dll_address != NULL && engine_address != NULL;
}

bool NavmeshPoints::load_config()
{
	static constexpr int DEC = 10;
	static constexpr int HEX = 16;

	config.windowname = FileReader::read_value_of_string_in_file("config.txt", "windowName");
	config.client_dll_name = FileReader::read_value_of_string_in_file("config.txt", "client Dll Name");
	config.engine_dll_name = FileReader::read_value_of_string_in_file("config.txt", "engine Dll Name");
	config.delay = stol(FileReader::read_value_of_string_in_file("config.txt", "delay"), NULL, DEC);
	config.trigger_button = stol(FileReader::read_value_of_string_in_file("config.txt", "triggerButton"), NULL, HEX);

	if (config.windowname == "" || config.client_dll_name == "" || config.trigger_button == 0)
		return false;

	save_button.set_toggle_button(config.trigger_button);

	return true;
}

void NavmeshPoints::run()
{

	while (!close_button.was_clicked())
	{
		close_button.update();
		save_button.update();
		auto position = get_current_position();

		if (save_button.was_clicked()) 
		{
			put_vec3d_in_vec(position);
			position.print();
		}
	}
	save_to_file();
}

void NavmeshPoints::save_to_file()
{
	std::ofstream my_file;
	my_file.open("navmesh.json");
	json nav_json = json::parse(R"({"nodes" : []})");

	for (int i = 0; i < this->points.size(); i++) 
	{
		nav_json["nodes"][i]["x"] = this->points[i].x;
		nav_json["nodes"][i]["y"] = this->points[i].y;
		nav_json["nodes"][i]["z"] = this->points[i].z;
		nav_json["nodes"][i]["id"] = i;
		nav_json["nodes"][i]["corner"] = (i < 2);
	}
	my_file << nav_json;

	my_file.close();
}

Vec3D<float> NavmeshPoints::get_current_position()
{
	DWORD engine_client_state_address = mem_manager.read_memory<DWORD>(engine_address + Offsets::clientState);
	DWORD player_address = mem_manager.read_memory<DWORD>(client_dll_address + Offsets::local_player_offset);

	auto player_position = mem_manager.read_memory<Vec3D<float>>(player_address + Offsets::position);
	auto buf = mem_manager.read_memory<Vec3D<float>>(engine_client_state_address + Offsets::position);

	return player_position;
}

void NavmeshPoints::put_vec3d_in_vec(const Vec3D<float>& vec)
{
	static int count = 0;
	this->points.push_back(vec);
	count++;
}

template<typename T>
inline std::string NavmeshPoints::to_string_with_max_precision(const T& a_value)
{
	int maxPrecision = std::numeric_limits<T>::max_digits10;

	std::stringstream out;
	out.precision(maxPrecision);
	out << a_value;
	return out.str();
}