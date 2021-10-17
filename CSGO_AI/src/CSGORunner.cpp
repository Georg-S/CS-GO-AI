#include "CSGORunner.h"

CSGORunner::CSGORunner(std::shared_ptr<CSGOAi> csgo_ai_handler) : QObject(nullptr), csgo_ai_handler(csgo_ai_handler)
{
	this->csgo_navmesh_points_handler = std::make_shared<NavmeshPoints>(csgo_ai_handler->get_logger(), csgo_ai_handler->get_game_info_handler());
}

void CSGORunner::run()
{
	while (is_running)
	{
		if (run_navmesh_points)
		{
			mutex.lock();
			bool new_point_added = csgo_navmesh_points_handler->update();
			mutex.unlock();
			if (new_point_added)
				emit new_point();
		}
		else
		{
			csgo_ai_handler->update();
		}
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
	emit new_point(); // emit outside of mutex, since we call it from the same thread 
}

Vec3D<float> CSGORunner::get_latest_point()
{
	mutex.lock();
	auto result = csgo_navmesh_points_handler->get_latest_point();
	mutex.unlock();

	return result;
}