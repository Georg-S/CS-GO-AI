#include "CS2/NavmeshPoints.h"

NavmeshPoints::NavmeshPoints(std::shared_ptr<GameInformationhandler> game_info_handler)
{
	m_game_info_handler = game_info_handler;
}

bool NavmeshPoints::update()
{
	if (!m_game_info_handler->is_attached_to_process())
		return false;

	m_save_button.update();

	if (!m_save_button.was_clicked())
		return false;

	add_point();

	return true;
}

void NavmeshPoints::add_point()
{
	if (!m_game_info_handler->is_attached_to_process())
		return;

	m_game_info_handler->update_game_information();
	auto game_info = m_game_info_handler->get_game_information();
	auto position = game_info.controlled_player.position;

	if (m_map_name == "") 
	{
		m_map_name = std::string(game_info.current_map);
		std::replace(m_map_name.begin(), m_map_name.end(), '/', '_');
	}

	m_points.push_back(position);
	Logging::log("Point added: " + position.to_string());
}

void NavmeshPoints::set_add_point_button(int key_code)
{
	m_save_button.set_toggle_button(key_code);
}

bool NavmeshPoints::save_to_file()
{
	try
	{
		std::ofstream my_file;
		std::string file_name = "Navmesh/json/";
		if (m_map_name == "")
			file_name = file_name + "no_name" + ".json";
		else
			file_name = file_name + m_map_name + ".json";

		my_file.open(file_name);
		json nav_json;
		nav_json["map_name"] = m_map_name;

		for (unsigned int i = 0; i < m_points.size(); i++)
		{
			nav_json["nodes"][i]["x"] = m_points[i].x;
			nav_json["nodes"][i]["y"] = m_points[i].y;
			nav_json["nodes"][i]["z"] = m_points[i].z;
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
	if (!m_game_info_handler->is_attached_to_process())
		return std::pair<bool, Vec3D<float>>(false, Vec3D<float>());

	m_game_info_handler->update_game_information();
	auto game_info = m_game_info_handler->get_game_information();
	auto position = game_info.controlled_player.position;

	return std::pair<bool, Vec3D<float>>(true, position);
}
