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
	uintptr_t local_player_controller_offset;
	uintptr_t local_player_pawn;
	uintptr_t crosshair_offset;
	uintptr_t shots_fired_offset;
	uintptr_t entity_list_start_offset;
	uintptr_t player_health_offset;
	uintptr_t team_offset;
	uintptr_t entity_listelement_size;
	uintptr_t position;
	uintptr_t force_attack;
	uintptr_t force_forward;
	uintptr_t force_backward;
	uintptr_t force_left;
	uintptr_t force_right;
	uintptr_t model_state;
	uintptr_t sceneNode;
	uintptr_t player_pawn_handle;

	//engine.dll
	uintptr_t client_state_view_angle;
	uintptr_t current_map;
};

std::optional <Offsets> load_offsets_from_files();
