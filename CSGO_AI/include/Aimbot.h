#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include "GameInformationHandler.h"

class Aimbot
{
public:
	void update(GameInformationhandler* info_handler);

private:
	Vec3D<float> get_closest_enemy(const GameInformation& game_info);
	Vec2D<float> calc_view_vec_aim_to_head(const Vec3D<float>& player_head, const Vec3D<float>& enemy_head);
};