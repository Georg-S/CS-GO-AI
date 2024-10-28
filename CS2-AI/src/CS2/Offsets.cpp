#include "CS2/Offsets.h"

using json = nlohmann::json;
static constexpr int HEX = 16;
static json offsets_json;


std::optional<Offsets> load_offsets_from_file(const std::string& filename)
{
	auto offsets = Offsets{};
	try
	{
		std::ifstream ifs(filename);
		auto offsets_json = json::parse(ifs);
		ifs.close();

		offsets.entity_listelement_size = 0x10;
		offsets.local_player_offset = (DWORD)offsets_json["signatures"]["dwLocalPlayer"];
		offsets.crosshair_offset = (DWORD)offsets_json["netvars"]["m_iCrosshairId"];
		offsets.shots_fired_offset = (DWORD)offsets_json["netvars"]["m_iShotsFired"];
		offsets.entity_list_start_offset = (DWORD)offsets_json["signatures"]["dwEntityList"];
		offsets.player_health_offset = (DWORD)offsets_json["netvars"]["m_iHealth"];
		offsets.team_offset = (DWORD)offsets_json["netvars"]["m_iTeamNum"];
		offsets.position = (DWORD)offsets_json["netvars"]["m_vecOrigin"];
		offsets.client_state = (DWORD)offsets_json["signatures"]["dwClientState"];
		offsets.client_state_view_angle = (DWORD)offsets_json["signatures"]["dwClientState_ViewAngles"];
		offsets.client_state_max_players = (DWORD)offsets_json["signatures"]["dwClientState_MaxPlayer"];
		offsets.force_attack = (DWORD)offsets_json["signatures"]["dwForceAttack"];

		offsets.force_forward = (DWORD)offsets_json["signatures"]["dwForceForward"];
		offsets.force_backward = (DWORD)offsets_json["signatures"]["dwForceBackward"];
		offsets.force_left = (DWORD)offsets_json["signatures"]["dwForceLeft"];
		offsets.force_right = (DWORD)offsets_json["signatures"]["dwForceRight"];

		offsets.bone_matrix = (DWORD)offsets_json["netvars"]["m_dwBoneMatrix"];
		offsets.current_map = (DWORD)offsets_json["signatures"]["dwClientState_Map"];
	}
	catch (std::exception const& e)
	{
		Logging::log_error(std::string(e.what()));
		return {};
	}

	return offsets;
}
