#pragma once
#include <iostream>
#include <vector>
#include "Offsets.h"
#include "Vec3D.h"
#include "MemoryManager.h"
#include "ConfigReader.h"

struct PlayerInformation 
{
	Vec3D<float> pos;
	int health;
	int team;
};

struct GameInformation 
{
};

class GameInformationhandler 
{
public:
	GameInformationhandler(const ConfigData& configData);
	void update_game_information();
	GameInformation get_game_information() const;

private:
	GameInformation gameInformation;
};