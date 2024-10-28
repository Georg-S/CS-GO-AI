#include "CS2/Aimbot.h"


void Aimbot::update(GameInformationhandler* info_handler)
{
	if (!info_handler)
		return;

	GameInformation game_info = info_handler->get_game_information();
	if (!game_info.closest_enemy_player)
		return;

	Vec3D<float> closest_enemy_head = game_info.closest_enemy_player->head_position;
	Vec2D<float> new_view_vec = calc_view_vec_aim_to_head(game_info.controlled_player.head_position, closest_enemy_head);
	info_handler->set_view_vec(new_view_vec);
}

Vec2D<float> Aimbot::calc_view_vec_aim_to_head(const Vec3D<float>& player_head, const Vec3D<float>& enemy_head)
{
	const Vec3D<float> vec_to_enemy = enemy_head - player_head;
	const auto z_vec = Vec3D<float>(0, 0, 1);

	float cos = z_vec.dot_product(vec_to_enemy) / (z_vec.calc_abs() * vec_to_enemy.calc_abs());
	float vertical_angle = acos(cos) / PI * 180;
	vertical_angle -= 90;

	Vec2D<float> result = {};
	result.x = vertical_angle;
	result.y = atan2(vec_to_enemy.y, vec_to_enemy.x) / PI * 180;

	return result;
}
