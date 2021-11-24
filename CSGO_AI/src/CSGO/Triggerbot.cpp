#include "CSGO/Triggerbot.h"

void Triggerbot::update(GameInformationhandler* handler)
{
	if (!handler)
		return;

	const GameInformation game_info = handler->get_game_information();

	if (game_info.controlled_player.shots_fired > 0)
		handler->set_player_shooting(false);

	if (!game_info.player_in_crosshair)
		return;

	if (game_info.player_in_crosshair->team == game_info.controlled_player.team)
		return;

	if ((game_info.player_in_crosshair->health <= 0) || (game_info.player_in_crosshair->health >=200))
		return;

	auto current_time = std::chrono::system_clock::now();
	auto current_time_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(current_time).time_since_epoch().count();
	
	constexpr int delay_in_ms = 500;
	if (current_time_ms >= delay_time)
	{
		delay_time = current_time_ms + delay_in_ms;
		handler->set_player_shooting(true);
	}
}
