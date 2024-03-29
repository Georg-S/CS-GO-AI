#pragma once
#include <iostream>
#include <vector>
#include "Offsets.h"
#include "Utility/Vec3D.h"
#include "Utility/Vec2D.h"
#include "MemoryManager.h"
#include "ConfigReader.h"

struct Movement
{
	bool forward = false;
	bool backward = false;
	bool left = false;
	bool right = false;
};

struct ControlledPlayer 
{
	Vec2D<float> view_vec;
	Vec3D<float> position;
	Vec3D<float> head_position;
	Movement movement;
	DWORD shooting;
	DWORD shots_fired;
	int team;
	int health;
};

struct PlayerInformation 
{
	Vec3D<float> position;
	Vec3D<float> head_position;
	int team;
	int health;
};

struct GameInformation 
{
	ControlledPlayer controlled_player;
	std::vector<PlayerInformation> other_players;
	std::shared_ptr<PlayerInformation> player_in_crosshair = nullptr;
	std::shared_ptr<PlayerInformation> closest_enemy_player = nullptr;
	char current_map[64] = "";
};

class GameInformationhandler
{
public:
	GameInformationhandler() = default;
	bool init(const Config& config);
	bool loadOffsets(const std::string& file_name);
	void update_game_information();

	GameInformation get_game_information() const;
	bool is_attached_to_process()const;
	void set_view_vec(const Vec2D<float>& view_vec);
	void set_player_movement(const Movement& movement);
	void set_player_shooting(bool val);

private:
	ControlledPlayer read_controlled_player_information(DWORD player_address, DWORD engine_client_state_address);
	std::vector<PlayerInformation> read_other_players(DWORD player_address, DWORD engine_client_state_address);
	Movement read_controlled_player_movement(DWORD player_address);
	Vec3D<float> get_head_bone_position(DWORD entity);
	std::shared_ptr<PlayerInformation> read_player_in_crosshair(DWORD player_address);
	std::shared_ptr<PlayerInformation> get_closest_enemy(const GameInformation& game_info);
	void read_in_current_map(DWORD engine_client_state_address, char* buffer, DWORD buffer_size);
	bool read_in_if_controlled_player_is_shooting();

	bool attached_to_process = false;
	GameInformation game_information;
	MemoryManager mem_manager;
	DWORD client_dll_address = 0;
	DWORD engine_address = 0;
	Offsets offsets = {};
};