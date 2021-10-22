#include "CSGORunner.h"

CSGORunner::CSGORunner(): QObject(nullptr)
{
	csgo_ai_handler = std::make_unique<CSGOAi>();
	this->csgo_navmesh_points_handler = std::make_unique<NavmeshPoints>(csgo_ai_handler->get_game_info_handler());
	load_files();
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
	if (run_navmesh_points)
		csgo_navmesh_points_handler->update();
	else
		csgo_ai_handler->update();
}

void CSGORunner::set_run_navmesh_points(bool value)
{
	QMutexLocker lock(&mutex);
	this->run_navmesh_points = value;
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

void CSGORunner::load_files()
{
	QMutexLocker lock(&mutex);
	if (!csgo_ai_handler->load_config("config.json"))
		Logging::log_error("Config couldn't be read, make sure you have a valid config");
	if (!csgo_ai_handler->load_offsets("offsets.json"))
		Logging::log_error("Offsets couldn't be read, make sure you have a valid offsets file");
	if (!csgo_ai_handler->load_navmesh("nav_mesh.json"))
		Logging::log_error("Error loading / parsing Navmesh, make sure you have a valid nav - mesh file");
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
