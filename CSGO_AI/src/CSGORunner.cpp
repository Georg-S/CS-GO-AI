#include "CSGORunner.h"

CSGORunner::CSGORunner(std::shared_ptr<CSGOAi> csgo_ai_handler) : QObject(nullptr), csgo_ai_handler(csgo_ai_handler)
{
	this->csgo_navmesh_points_handler = std::make_shared<NavmeshPoints>(csgo_ai_handler->get_game_info_handler());
}

void CSGORunner::run()
{
	while (is_running)
	{
		mutex.lock();
		if (run_navmesh_points)
			csgo_navmesh_points_handler->update();
		else
			csgo_ai_handler->update();
		mutex.unlock();
	}

	deleteLater();
}

void CSGORunner::set_run_navmesh_points(bool value)
{
	this->run_navmesh_points = value;
}

void CSGORunner::set_add_point_key(int key_code)
{
	csgo_navmesh_points_handler->set_add_point_button(key_code);
}

bool CSGORunner::save_navmesh_points()
{
	mutex.lock();
	bool success = this->csgo_navmesh_points_handler->save_to_file();
	mutex.unlock();

	return success;
}

void CSGORunner::add_point()
{
	mutex.lock();
	csgo_navmesh_points_handler->add_point();
	mutex.unlock();
}

void CSGORunner::load_files()
{
}

void CSGORunner::attach_to_process()
{
}
