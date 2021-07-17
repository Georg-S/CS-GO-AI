#include "Triggerbot.h"

void Triggerbot::update(GameInformationhandler* handler)
{
	if (!handler)
		return;

	GameInformation game_info = handler->get_game_information();

	if (!game_info.player_in_crosshair)
		return;

	if (game_info.player_in_crosshair->team == game_info.controlled_player.team)
		return;

	if ((game_info.player_in_crosshair->health <= 0) || (game_info.player_in_crosshair->health >=200))
		return;

	simulate_left_click();
}

void Triggerbot::simulate_left_click()
{
	mouse_event(MOUSEEVENTF_LEFTDOWN, NULL, NULL, NULL, NULL);
	mouse_event(MOUSEEVENTF_LEFTUP, NULL, NULL, NULL, NULL);
}
