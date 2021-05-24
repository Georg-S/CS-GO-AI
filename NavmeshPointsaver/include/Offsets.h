#pragma once
#include <memory>
#include "FileReader.h"
#include <wtypes.h>

class Offsets
{
public:
	static bool load_offsets_from_file();

	//Will be read from the Offset file
	//client.dll
	static DWORD local_player_offset;
	static DWORD crosshair_offset;
	static DWORD entity_list_start_offset;
	static DWORD player_health_offset;
	static DWORD team_offset;
	static DWORD entity_listelement_size;
	static DWORD position;

	//engine.dll
	static DWORD clientState;
	static DWORD client_state_view_angle;
	static DWORD client_state_max_players;
	static DWORD bone_matrix;

private:
	static bool initialized_succesful;
	static constexpr int HEX = 16;
};

