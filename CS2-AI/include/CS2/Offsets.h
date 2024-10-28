#pragma once
#include <wtypes.h>
#include <fstream>
#include <optional>
#include "Utility/json.hpp"
#include "Utility/Logging.h"

struct Offsets
{
public:
	//Will be read from the Offset file
		//client.dll
	DWORD local_player_offset;
	DWORD crosshair_offset;
	DWORD shots_fired_offset;
	DWORD entity_list_start_offset;
	DWORD player_health_offset;
	DWORD team_offset;
	DWORD entity_listelement_size;
	DWORD position;
	DWORD force_attack;
	DWORD force_forward;
	DWORD force_backward;
	DWORD force_left;
	DWORD force_right;

	//engine.dll
	DWORD client_state;
	DWORD client_state_view_angle;
	DWORD client_state_max_players;
	DWORD bone_matrix;
	DWORD current_map;
};

std::optional <Offsets> load_offsets_from_file(const std::string& filename);
