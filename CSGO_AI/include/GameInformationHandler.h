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
	GameInformationhandler();
	bool init(const ConfigData& config);
	void update_game_information();

	GameInformation get_game_information() const;


private:
	ConfigData config_data;
	GameInformation game_information;
	MemoryManager mem_manager;

	DWORD client_dll_address;
	DWORD engine_address;
};