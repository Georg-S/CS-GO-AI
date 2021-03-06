#include "UI/CSGORunner.h"

CSGORunner::CSGORunner(): QObject(nullptr)
{
	csgo_ai_handler = std::make_unique<CSGOAi>();
	csgo_navmesh_points_handler = std::make_unique<NavmeshPoints>(csgo_ai_handler->get_game_info_handler());
	load_config();
	load_offsets();
	attach_to_process();
}

void CSGORunner::run()
{
	while (is_running)
		update();

	deleteLater();
}

void CSGORunner::update()
{
	QMutexLocker lock(&mutex);
	if(mode == ModeRunning::AI)
		csgo_ai_handler->update();
	else if (mode == ModeRunning::POINT_CREATOR)
		csgo_navmesh_points_handler->update();
}

void CSGORunner::set_mode(ModeRunning mode)
{
	QMutexLocker lock(&mutex);
	this->mode = mode;
}

void CSGORunner::set_add_point_key(int key_code)
{
	QMutexLocker lock(&mutex);
	csgo_navmesh_points_handler->set_add_point_button(key_code);
}

bool CSGORunner::save_navmesh_points()
{
	QMutexLocker lock(&mutex);
	bool success = this->csgo_navmesh_points_handler->save_to_file();

	return success;
}

void CSGORunner::add_point()
{
	QMutexLocker lock(&mutex);
	csgo_navmesh_points_handler->add_point();
}

void CSGORunner::load_config()
{
	QMutexLocker lock(&mutex);
	csgo_ai_handler->load_config("config.json");
}

void CSGORunner::load_offsets()
{
	QMutexLocker lock(&mutex);
	csgo_ai_handler->load_offsets("offsets.json");
}

void CSGORunner::load_navmesh()
{
	QMutexLocker lock(&mutex);
	csgo_ai_handler->load_navmesh();
}

void CSGORunner::attach_to_process()
{
	QMutexLocker lock(&mutex);
	if (!csgo_ai_handler->attach_to_csgo_process())
		Logging::log_error("Error getting dll address / Error attaching to CS-GO process");
	else
		Logging::log_success("Attached to the CSGO process");
}

void CSGORunner::set_activated_behavior(const ActivatedFeatures& behavior)
{
	QMutexLocker lock(&mutex);
	csgo_ai_handler->set_activated_behavior(behavior);
}

std::pair<bool, Vec3D<float>> CSGORunner::get_current_position()
{
	QMutexLocker lock(&mutex);
	return csgo_navmesh_points_handler->get_current_position();
}
