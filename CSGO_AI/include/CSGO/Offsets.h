#pragma once
#include <wtypes.h>
#include <fstream>
#include "Utility/json.hpp"
#include "Utility/Logging.h"

using json = nlohmann::json;

class Offsets
{
public:
	static bool load_offsets_from_file(const std::string& filename);

//Will be read from the Offset file
	//client.dll
	static DWORD local_player_offset;
	static DWORD crosshair_offset;
	static DWORD shots_fired_offset;
	static DWORD entity_list_start_offset;
	static DWORD player_health_offset;
	static DWORD team_offset;
	static DWORD entity_listelement_size;
	static DWORD position;
	static DWORD force_attack;
	static DWORD force_forward;
	static DWORD force_backward;
	static DWORD force_left;
	static DWORD force_right;

	//engine.dll
	static DWORD client_state;
	static DWORD client_state_view_angle;
	static DWORD client_state_max_players;
	static DWORD bone_matrix;
	static DWORD current_map;

private:
	static constexpr int HEX = 16;
	static json offsets_json;
};

