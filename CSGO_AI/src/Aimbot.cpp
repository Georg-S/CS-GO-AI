#include "Aimbot.h"

void Aimbot::update(const GameInformation& game_info)
{
}

Vec3D<float> Aimbot::get_closest_enemy(const GameInformation& game_info)
{
	Vec3D<float> closest_vec;
	float closest_distance = FLT_MAX;

	for (const auto& enemy : game_info.other_players)
	{
		Vec3D<float> buf = game_info.controlled_player.head_position - enemy.head_position;
		float distance = buf.calc_abs();

		if ((distance <= closest_distance) && (enemy.team != game_info.controlled_player.team) && (enemy.health > 0))
		{
			closest_distance = distance;
			closest_vec = enemy.head_position;
		}
	}

	return closest_vec;
}
