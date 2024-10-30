#include "UI/CS2Runner.h"

CS2Runner::CS2Runner(): QObject(nullptr)
{
	cs2_ai_handler = std::make_unique<CS2Ai>();
	cs2_navmesh_points_handler = std::make_unique<NavmeshPoints>(cs2_ai_handler->get_game_info_handler());
	load_config();
	load_offsets();
	attach_to_process();
}

void CS2Runner::run()
{
	while (is_running) 
	{
		update();
		// 1 ms sleep because else activating the behaviors would take too long since the lock is taken all the time
		// Could be solved by e.g. making the "ActivatedFeatures" atomic but for now the sleep should be enough
		Sleep(1); 
	}

	deleteLater();
}

void CS2Runner::update()
{
	std::scoped_lock lock(mutex);
	if(mode == ModeRunning::AI)
		cs2_ai_handler->update();
	else if (mode == ModeRunning::POINT_CREATOR)
		cs2_navmesh_points_handler->update();
}

void CS2Runner::set_mode(ModeRunning mode)
{
	std::scoped_lock lock(mutex);
	this->mode = mode;
}

void CS2Runner::set_add_point_key(int key_code)
{
	std::scoped_lock lock(mutex);
	cs2_navmesh_points_handler->set_add_point_button(key_code);
}

bool CS2Runner::save_navmesh_points()
{
	std::scoped_lock lock(mutex);
	bool success = this->cs2_navmesh_points_handler->save_to_file();

	return success;
}

void CS2Runner::add_point()
{
	std::scoped_lock lock(mutex);
	cs2_navmesh_points_handler->add_point();
}

void CS2Runner::load_config()
{
	std::scoped_lock lock(mutex);
	cs2_ai_handler->load_config();
}

void CS2Runner::load_offsets()
{
	std::scoped_lock lock(mutex);
	cs2_ai_handler->load_offsets();
}

void CS2Runner::load_navmesh()
{
	std::scoped_lock lock(mutex);
	cs2_ai_handler->load_navmesh();
}

void CS2Runner::attach_to_process()
{
	std::scoped_lock lock(mutex);
	if (!cs2_ai_handler->attach_to_cs2_process())
		Logging::log_error("Error getting dll address / Error attaching to CS2 process");
	else
		Logging::log_success("Attached to the CS2 process");
}

void CS2Runner::set_activated_behavior(const ActivatedFeatures& behavior)
{
	std::scoped_lock lock(mutex);
	cs2_ai_handler->set_activated_behavior(behavior);
}

std::pair<bool, Vec3D<float>> CS2Runner::get_current_position()
{
	std::scoped_lock lock(mutex);
	return cs2_navmesh_points_handler->get_current_position();
}
