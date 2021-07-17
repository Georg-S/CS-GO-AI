#include "Aimbot.h"


void Aimbot::update(GameInformationhandler* info_handler)
{
	if (!info_handler)
		return;

	GameInformation game_info = info_handler->get_game_information();
	Vec3D<float> closest_enemy = get_closest_enemy(game_info);
	Vec2D<float> new_view_vec = calc_view_vec_aim_to_head(game_info.controlled_player.head_position, closest_enemy);
	info_handler->set_view_vec(new_view_vec);
}

Vec3D<float> Aimbot::get_closest_enemy(const GameInformation& game_info)
{
	Vec3D<float> closest_vec;
	const auto& controlled_player = game_info.controlled_player;
	float closest_distance = FLT_MAX;

	for (const auto& enemy : game_info.other_players)
	{
		Vec3D<float> buf = controlled_player.head_position - enemy.head_position;
		float distance = buf.calc_abs();

		if ((distance <= closest_distance) && (enemy.team != controlled_player.team) && (enemy.health > 0))
		{
			closest_distance = distance;
			closest_vec = enemy.head_position;
		}
	}

	return closest_vec;
}

Vec2D<float> Aimbot::calc_view_vec_aim_to_head(const Vec3D<float>& player_head, const Vec3D<float>& enemy_head)
{
	Vec2D<float> result;
	const Vec3D<float> vec_to_enemy = enemy_head - player_head;
	const Vec3D<float> z_vec{0, 0, 1};

	float cos = z_vec.dot_product(vec_to_enemy) / (z_vec.calc_abs() * vec_to_enemy.calc_abs());
	float vertical_angle = acos(cos) / M_PI * 180;
	vertical_angle -= 90;

	result.x = vertical_angle;
	result.y = atan2(vec_to_enemy.y, vec_to_enemy.x) / M_PI * 180;

	return result;
}
