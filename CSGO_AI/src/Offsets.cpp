#include "Offsets.h"

DWORD Offsets::local_player_offset;
DWORD Offsets::crosshair_offset;
DWORD Offsets::entity_list_start_offset;
DWORD Offsets::player_health_offset;
DWORD Offsets::team_offset;
DWORD Offsets::entity_listelement_size;
DWORD Offsets::position;
DWORD Offsets::client_state;
DWORD Offsets::client_state_view_angle;
DWORD Offsets::client_state_max_players;
DWORD Offsets::bone_matrix;
DWORD Offsets::force_attack;
DWORD Offsets::force_forward;
DWORD Offsets::force_backward;
DWORD Offsets::force_left;
DWORD Offsets::force_right;
DWORD Offsets::current_map;

json Offsets::offsets_json;

bool Offsets::load_offsets_from_file(const std::string& filename)
{
	try
	{
		std::ifstream ifs(filename);
		Offsets::offsets_json = json::parse(ifs);
		ifs.close();

		entity_listelement_size = 0x10;
		local_player_offset = (DWORD)offsets_json["signatures"]["dwLocalPlayer"];
		crosshair_offset = (DWORD)offsets_json["netvars"]["m_iCrosshairId"];
		entity_list_start_offset = (DWORD)offsets_json["signatures"]["dwEntityList"];
		player_health_offset = (DWORD)offsets_json["netvars"]["m_iHealth"];
		team_offset = (DWORD)offsets_json["netvars"]["m_iTeamNum"];
		position = (DWORD)offsets_json["netvars"]["m_vecOrigin"];
		client_state = (DWORD)offsets_json["signatures"]["dwClientState"];
		client_state_view_angle = (DWORD)offsets_json["signatures"]["dwClientState_ViewAngles"];
		client_state_max_players = (DWORD)offsets_json["signatures"]["dwClientState_MaxPlayer"];
		force_attack = (DWORD)offsets_json["signatures"]["dwForceAttack"];


		force_forward = (DWORD)offsets_json["signatures"]["dwForceForward"];
		force_backward = (DWORD)offsets_json["signatures"]["dwForceBackward"];
		force_left = (DWORD)offsets_json["signatures"]["dwForceLeft"];
		force_right = (DWORD)offsets_json["signatures"]["dwForceRight"];

		bone_matrix = (DWORD)offsets_json["netvars"]["m_dwBoneMatrix"];
		current_map = (DWORD)offsets_json["signatures"]["dwClientState_Map"];

	}
	catch (std::exception const& e)
	{
		std::cout << e.what() << std::endl;
		return false;
	}
	return true;
}
