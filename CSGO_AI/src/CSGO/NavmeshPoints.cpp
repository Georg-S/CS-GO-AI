#include "CSGO/NavmeshPoints.h"

NavmeshPoints::NavmeshPoints(std::shared_ptr<GameInformationhandler> game_info_handler)
{
	this->game_info_handler = game_info_handler;
}

bool NavmeshPoints::init()
{
	auto readConfig = Config::read_in_config_data("config.json");
	if (!readConfig)
	{
		Logging::log_error("Loading config failed, check config file");
		return false;
	}
	config = readConfig.value();

	if (!Offsets::load_offsets_from_file("offsets.json"))
	{
		Logging::log_error("Loading offsets failed, check offsets file");
		return false;
	}

	game_info_handler = std::make_shared<GameInformationhandler>();
	if (!game_info_handler->init(config))
	{
		Logging::log_error("Error getting dll address ");
		return false;
	}

	return true;
}

bool NavmeshPoints::update()
{
	if (!game_info_handler->is_attached_to_process())
		return false;

	save_button.update();

	if (!save_button.was_clicked())
		return false;

	add_point();

	return true;
}

void NavmeshPoints::add_point()
{
	if (!game_info_handler->is_attached_to_process())
		return;

	game_info_handler->update_game_information();
	auto game_info = game_info_handler->get_game_information();
	auto position = game_info.controlled_player.position;

	if (map_name == "") 
	{
		map_name = std::string(game_info.current_map);
		std::replace(map_name.begin(), map_name.end(), '/', '_');
	}

	points.push_back(position);
	Logging::log("Point added: " + position.to_string());
}

void NavmeshPoints::set_add_point_button(int key_code)
{
	save_button.set_toggle_button(key_code);
}

bool NavmeshPoints::save_to_file()
{
	try
	{
		std::ofstream my_file;
		std::string file_name = "Navmesh/json/";
		if (map_name == "")
			file_name = file_name + "no_name" + ".json";
		else
			file_name = file_name + map_name + ".json";

		my_file.open(file_name);
		json nav_json;
		nav_json["map_name"] = map_name;

		for (unsigned int i = 0; i < this->points.size(); i++)
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
	catch (std::exception const& e)
	{
		Logging::log_error(std::string(e.what()));
		return false;
	}
	return true;
}

std::pair<bool, Vec3D<float>> NavmeshPoints::get_current_position() const
{
	if (!game_info_handler->is_attached_to_process())
		return std::pair<bool, Vec3D<float>>(false, Vec3D<float>());

	game_info_handler->update_game_information();
	auto game_info = game_info_handler->get_game_information();
	auto position = game_info.controlled_player.position;

	return std::pair<bool, Vec3D<float>>(true, position);
}
