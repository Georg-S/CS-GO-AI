#include "CS2/Offsets.h"

#include "CS2/Constants.h"

using json = nlohmann::json;
static constexpr int HEX = 16;
static json offsets_json;


std::optional<Offsets> load_offsets_from_files()
{
	auto offsets = Offsets{};
	try
	{
		std::ifstream general_stream(general_offsets_file_path);
		std::ifstream client_stream(client_offsets_file_path);
		std::ifstream engine_stream(engine_offsets_file_path);
		std::ifstream buttons_stream(buttons_offsets_file_path);

		auto general_offsets_json = json::parse(general_stream);
		auto client_offsets_json = json::parse(client_stream);
		auto engine_offsets_json = json::parse(engine_stream);
		auto buttons_offsets_json = json::parse(buttons_stream);

		offsets.entity_listelement_size = 0x10;
		offsets.local_player_controller_offset = static_cast<uintptr_t>(general_offsets_json["client.dll"]["dwLocalPlayerController"]);
		offsets.entity_list_start_offset = static_cast<uintptr_t>(general_offsets_json["client.dll"]["dwEntityList"]);
		offsets.local_player_pawn = static_cast<uintptr_t>(general_offsets_json["client.dll"]["dwLocalPlayerPawn"]);
		offsets.global_vars = static_cast<uintptr_t>(general_offsets_json["client.dll"]["dwGlobalVars"]);
		offsets.client_state_view_angle = static_cast<uintptr_t>(general_offsets_json["client.dll"]["dwViewAngles"]);

		offsets.player_health_offset = static_cast<uintptr_t>(client_offsets_json["client.dll"]["classes"]["C_BaseEntity"]["fields"]["m_iHealth"]);
		offsets.player_pawn_handle = static_cast<uintptr_t>(client_offsets_json["client.dll"]["classes"]["CCSPlayerController"]["fields"]["m_hPlayerPawn"]);
		offsets.team_offset = static_cast<uintptr_t>(client_offsets_json["client.dll"]["classes"]["C_BaseEntity"]["fields"]["m_iTeamNum"]);
		offsets.sceneNode = static_cast<uintptr_t>(client_offsets_json["client.dll"]["classes"]["C_BaseEntity"]["fields"]["m_pGameSceneNode"]);
		offsets.position = static_cast<uintptr_t>(client_offsets_json["client.dll"]["classes"]["C_BasePlayerPawn"]["fields"]["m_vOldOrigin"]);
		offsets.model_state = static_cast<uintptr_t>(client_offsets_json["client.dll"]["classes"]["CSkeletonInstance"]["fields"]["m_modelState"]);

		offsets.shots_fired_offset = static_cast<uintptr_t>(client_offsets_json["client.dll"]["classes"]["C_CSPlayerPawn"]["fields"]["m_iShotsFired"]);

		offsets.force_attack = static_cast<uintptr_t>(buttons_offsets_json["client.dll"]["attack"]);
		offsets.force_forward = static_cast<uintptr_t>(buttons_offsets_json["client.dll"]["forward"]);
		offsets.force_backward = static_cast<uintptr_t>(buttons_offsets_json["client.dll"]["back"]);
		offsets.force_left = static_cast<uintptr_t>(buttons_offsets_json["client.dll"]["left"]);
		offsets.force_right = static_cast<uintptr_t>(buttons_offsets_json["client.dll"]["right"]);

		offsets.crosshair_offset = static_cast<uintptr_t>(client_offsets_json["client.dll"]["classes"]["C_CSPlayerPawnBase"]["fields"]["m_iIDEntIndex"]);

		//offsets.current_map = (DWORD)offsets_json["signatures"]["dwClientState_Map"];
	}
	catch (std::exception const& e)
	{
		Logging::log_error(std::string(e.what()));
		return {};
	}

	return offsets;
}
