#include "Offsets.h"

bool Offsets::initialized_succesful = false;
DWORD Offsets::local_player_offset;
DWORD Offsets::crosshair_offset;
DWORD Offsets::entity_list_start_offset;
DWORD Offsets::player_health_offset;
DWORD Offsets::team_offset;
DWORD Offsets::entity_listelement_size;
DWORD Offsets::position;
DWORD Offsets::clientState;
DWORD Offsets::client_state_view_angle;
DWORD Offsets::client_state_max_players;
DWORD Offsets::bone_matrix;

bool Offsets::load_offsets_from_file()
{
	try
	{
		local_player_offset = stol(FileReader::read_value_of_string_in_file("memoryOffsets.txt", "localPlayer"), NULL, HEX);
		crosshair_offset = stol(FileReader::read_value_of_string_in_file("memoryOffsets.txt", "crosshairOffset"), NULL, HEX);
		entity_list_start_offset = stol(FileReader::read_value_of_string_in_file("memoryOffsets.txt", "entityBase"), NULL, HEX);
		player_health_offset = stol(FileReader::read_value_of_string_in_file("memoryOffsets.txt", "PlayerHealth"), NULL, HEX);
		team_offset = stol(FileReader::read_value_of_string_in_file("memoryOffsets.txt", "teamOffset"), NULL, HEX);
		entity_listelement_size = stol(FileReader::read_value_of_string_in_file("memoryOffsets.txt", "EntLoopDist"), NULL, HEX);
		position = stol(FileReader::read_value_of_string_in_file("memoryOffsets.txt", "position"), NULL, HEX);
		clientState = stol(FileReader::read_value_of_string_in_file("memoryOffsets.txt", "clientStateBase"), NULL, HEX);
		client_state_view_angle = stol(FileReader::read_value_of_string_in_file("memoryOffsets.txt", "clientStateViewAngle"), NULL, HEX);
		client_state_max_players = stol(FileReader::read_value_of_string_in_file("memoryOffsets.txt", "clientStateMaxPlayer"), NULL, HEX);
		bone_matrix = stol(FileReader::read_value_of_string_in_file("memoryOffsets.txt", "boneMatrix"), NULL, HEX);
	}
	catch (std::exception const& e)
	{
		return false;
	}

	if (local_player_offset == 0 || crosshair_offset == 0 ||
		player_health_offset == 0 || team_offset == 0 || entity_listelement_size == 0 || clientState == 0)
	{
		std::cout << "In the offsets file one or more values are 0" << std::endl;
		return false;
	}
	return true;
}
