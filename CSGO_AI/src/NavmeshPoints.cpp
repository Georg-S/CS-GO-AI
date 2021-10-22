#include "NavmeshPoints.h"

NavmeshPoints::NavmeshPoints(std::shared_ptr<GameInformationhandler> game_info_handler)
{
	this->game_info_handler = game_info_handler;
}

bool NavmeshPoints::init()
{
	if (!ConfigReader::read_in_config_data(config, "config.json"))
	{
		Logging::log_error("Loading config failed, check config file");
		return false;
	}

	if (!Offsets::load_offsets_from_file("offsets.json"))
	{
		Logging::log_error("Loading offsets failed, check offsets file");
		return false;
	}

	this->game_info_handler = std::make_unique<GameInformationhandler>();
	if (!this->game_info_handler->init(config))
	{
		Logging::log_error("Error getting dll address ");
		return false;
	}

	return true;
}

void NavmeshPoints::console_run()
{
	while (!close_button.was_clicked())
	{
		close_button.update();
		save_button.update();
		game_info_handler->update_game_information();
		auto game_info = game_info_handler->get_game_information();
		auto position = game_info.controlled_player.position;

		if (save_button.was_clicked()) 
		{
			points.push_back(position);
			position.print();
		}
	}
	save_to_file();
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
	game_info_handler->update_game_information();
	auto game_info = game_info_handler->get_game_information();
	auto position = game_info.controlled_player.position;

	points.push_back(position);
	Logging::log("Point added: " + position.to_string());
}

void NavmeshPoints::set_add_point_button(int key_code)
{
	this->save_button.set_toggle_button(key_code);
}

bool NavmeshPoints::save_to_file()
{
	try
	{
		std::ofstream my_file;
		my_file.open("nav_points.json");
		json nav_json = json::parse(R"({"nodes" : []})");

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

template<typename T>
inline std::string NavmeshPoints::to_string_with_max_precision(const T& a_value)
{
	int maxPrecision = std::numeric_limits<T>::max_digits10;

	std::stringstream out;
	out.precision(maxPrecision);
	out << a_value;
	return out.str();
}