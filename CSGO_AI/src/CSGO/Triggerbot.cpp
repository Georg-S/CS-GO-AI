#include "CSGO/Triggerbot.h"

void Triggerbot::update(GameInformationhandler* handler)
{
	if (!handler)
		return;

	const GameInformation game_info = handler->get_game_information();

	if ((game_info.controlled_player.shots_fired > 0) || (game_info.controlled_player.health <= 0))
		handler->set_player_shooting(false);

	if (!game_info.player_in_crosshair)
		return;

	if (game_info.player_in_crosshair->team == game_info.controlled_player.team)
		return;

	if ((game_info.player_in_crosshair->health <= 0) || (game_info.player_in_crosshair->health >=200))
		return;

	const auto current_time_ms = get_current_time_in_ms();
	
	constexpr int delay_in_ms = 500;
	if (current_time_ms >= delay_time)
	{
		delay_time = current_time_ms + delay_in_ms;
		handler->set_player_shooting(true);
	}
}
