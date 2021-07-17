#pragma once
#include <iostream>
#include <vector>
#include "Offsets.h"
#include "Vec3D.h"
#include "Vec2D.h"
#include "MemoryManager.h"
#include "ConfigReader.h"

struct ControlledPlayer 
{
	Vec2D<float> view_vec;
	Vec3D<float> head_position;
	int team;
	int health;
};

struct PlayerInformation 
{
	Vec3D<float> head_position;
	int team;
	int health;
};

struct GameInformation 
{
	ControlledPlayer controlled_player;
	std::vector<PlayerInformation> other_players;
};

class GameInformationhandler
{
public:
	GameInformationhandler();
	bool init(const ConfigData& config);
	void update_game_information();

	GameInformation get_game_information() const;

private:
	ControlledPlayer read_controlled_player_information(DWORD player_address, DWORD engine_client_state_address);
	std::vector<PlayerInformation> read_other_players(DWORD player_address, DWORD engine_client_state_address);
	Vec3D<float> get_head_bone_position(DWORD entity);

	GameInformation game_information;
	ConfigData config_data;
	MemoryManager mem_manager;
	DWORD client_dll_address;
	DWORD engine_address;
};