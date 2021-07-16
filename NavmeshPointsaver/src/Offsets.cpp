#include "Offsets.h"

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
json Offsets::offsets_json;

bool Offsets::load_offsets_from_file()
{
	try
	{
		std::ifstream ifs("offsets.json");
		Offsets::offsets_json = json::parse(ifs);
		ifs.close();

		entity_listelement_size = 0x10;
		local_player_offset = (DWORD)offsets_json["signatures"]["dwLocalPlayer"];
		crosshair_offset = (DWORD)offsets_json["netvars"]["m_iCrosshairId"];
		entity_list_start_offset = (DWORD)offsets_json["signatures"]["dwEntityList"];
		player_health_offset = (DWORD)offsets_json["netvars"]["m_iHealth"];
		team_offset = (DWORD)offsets_json["netvars"]["m_iTeamNum"];
		position = (DWORD)offsets_json["netvars"]["m_vecOrigin"];
		clientState = (DWORD)offsets_json["signatures"]["dwClientState"];
		client_state_view_angle = (DWORD)offsets_json["signatures"]["dwClientState_ViewAngles"];
		client_state_max_players = (DWORD)offsets_json["signatures"]["dwClientState_MaxPlayer"];
		bone_matrix = (DWORD)offsets_json["netvars"]["m_dwBoneMatrix"];
	}
	catch (std::exception const& e)
	{
		std::cout << e.what() << std::endl;
		return false;
	}
	return true;
}
