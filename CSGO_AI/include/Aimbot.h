#pragma once
#include "GameInformationHandler.h"

class Aimbot
{
public:
	void update(const GameInformation& game_info);

private:
	Vec3D<float> get_closest_enemy(const GameInformation& game_info);
};