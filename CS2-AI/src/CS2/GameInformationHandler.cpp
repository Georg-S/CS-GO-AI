#include "CS2/GameInformationHandler.h"

#include <chrono>

#include "Utility/Logging.h"
#include "CS2/Constants.h"

bool GameInformationhandler::init(const Config& config)
{
	this->config = config;
	mem_manager.attach_to_process(config.windowname.c_str());

	client_dll_address = mem_manager.get_module_address(config.client_dll_name.c_str());
	engine_address = mem_manager.get_module_address(config.engine_dll_name.c_str());

	attached_to_process = client_dll_address && engine_address;

	return attached_to_process;
}

bool GameInformationhandler::loadOffsets()
{
	auto offsets = load_offsets_from_files();
	if (offsets)
		this->offsets = offsets.value();

	return offsets.has_value();
}

void GameInformationhandler::update_game_information()
{
	auto player_controller_address = mem_manager.read_memory<uintptr_t>(client_dll_address + offsets.local_player_controller_offset);
	auto player_pawn_address = mem_manager.read_memory<uintptr_t>(client_dll_address + offsets.local_player_pawn);
	auto engine_client_state = engine_address;

	game_information.controlled_player = read_controlled_player_information(player_controller_address, engine_client_state);
	game_information.player_in_crosshair = read_player_in_crosshair(player_controller_address, player_pawn_address);
	game_information.other_players = read_other_players(player_controller_address, engine_client_state);
	game_information.closest_enemy_player = get_closest_enemy(game_information);
	read_in_current_map(engine_client_state, game_information.current_map, std::size(game_information.current_map));
}

GameInformation GameInformationhandler::get_game_information() const
{
	return game_information;
}

bool GameInformationhandler::is_attached_to_process() const
{
	return attached_to_process;
}

void GameInformationhandler::set_view_vec(const Vec2D<float>& view_vec)
{
	if (isnan(view_vec.x) || isnan(view_vec.y))
		return;

	mem_manager.write_memory<Vec2D<float>>(client_dll_address + offsets.client_state_view_angle, view_vec);
}

void GameInformationhandler::set_player_movement(const Movement& movement)
{
	// Writing to "offsets.force_forward" etc. seems to not work, therefore spam key events to the CS2 process
	auto send_key_event = [this](bool value, DWORD key_code)
	{
		HWND hwnd = FindWindowA(nullptr, config.windowname.c_str());
		if (!hwnd)
			return;

		if (value)
			PostMessage(hwnd, WM_KEYDOWN, key_code, 0);
		else
			PostMessage(hwnd, WM_KEYUP, key_code, 0);
	};

	auto handle_key = [this, send_key_event](bool value, DWORD key_code)
	{
		send_key_event(value, key_code);
	};

	constexpr DWORD w_key_code = 0x57;
	constexpr DWORD s_key_code = 0x53;
	constexpr DWORD a_key_code = 0x41;
	constexpr DWORD d_key_code = 0x44;

	handle_key(movement.forward, w_key_code);
	handle_key(movement.backward, s_key_code);
	handle_key(movement.left, a_key_code);
	handle_key(movement.right, d_key_code);
}

void GameInformationhandler::set_player_shooting(bool val)
{
	constexpr DWORD not_shooting_value = 16777472;

	DWORD mem_val = val ? button_pressed_value : not_shooting_value;

	mem_manager.write_memory<DWORD>(client_dll_address + offsets.force_attack, mem_val);
}

std::optional<PlayerInformation> GameInformationhandler::get_closest_enemy(const GameInformation& game_info)
{
	std::optional<PlayerInformation> closest_enemy = {};
	const auto& controlled_player = game_info.controlled_player;
	float closest_distance = FLT_MAX;

	for (const auto& enemy : game_info.other_players)
	{
		float distance = controlled_player.head_position.distance(enemy.head_position);

		if ((distance <= closest_distance) && (enemy.team != controlled_player.team) && (enemy.health > 0))
		{
			closest_distance = distance;
			closest_enemy = enemy;
		}
	}

	return closest_enemy;
}

void GameInformationhandler::read_in_current_map(uintptr_t engine_client_state_address, char* buffer, size_t buffer_size)
{
	constexpr uintptr_t global_var_map = 0x180;

	auto global_vars = mem_manager.read_memory<uintptr_t>(client_dll_address + offsets.global_vars);
	auto map_name_ptr = mem_manager.read_memory<uintptr_t>(global_vars + global_var_map);

	mem_manager.read_string_from_memory(map_name_ptr, buffer, buffer_size);
}

bool GameInformationhandler::read_in_if_controlled_player_is_shooting()
{
	DWORD val = mem_manager.read_memory<DWORD>(client_dll_address + offsets.force_attack);

	return val == button_pressed_value;
}

ControlledPlayer GameInformationhandler::read_controlled_player_information(uintptr_t player_address, uintptr_t engine_client_state_address)
{
	ControlledPlayer dest{};
	dest.view_vec = mem_manager.read_memory<Vec2D<float>>(client_dll_address + offsets.client_state_view_angle);
	dest.team = mem_manager.read_memory<int>(player_address + offsets.team_offset);

	auto local_player_pawn = mem_manager.read_memory<uintptr_t>(client_dll_address + offsets.local_player_pawn);
	dest.health = mem_manager.read_memory<int>(local_player_pawn + offsets.player_health_offset);
	dest.position = mem_manager.read_memory<Vec3D<float>>(local_player_pawn + offsets.position);
	dest.shots_fired = mem_manager.read_memory<DWORD>(local_player_pawn + offsets.shots_fired_offset);
	dest.shooting = read_in_if_controlled_player_is_shooting();
	dest.movement = read_controlled_player_movement(player_address);
	dest.head_position = get_head_bone_position(local_player_pawn);

	return dest;
}

std::vector<PlayerInformation> GameInformationhandler::read_other_players(uintptr_t player_address, uintptr_t engine_client_state_address)
{
	constexpr size_t max_players = 64;
	std::vector<PlayerInformation> other_players;
	uintptr_t entity_list_start_address = mem_manager.read_memory<uintptr_t>(client_dll_address + offsets.entity_list_start_offset);
	if (!entity_list_start_address)
		return other_players;

	for (int i = 0; i < max_players; i++)
	{
		uintptr_t listEntity = get_list_entity(i, entity_list_start_address);
		if (!listEntity)
			continue;

		auto current_controller = get_entity_controller_or_pawn(listEntity, i);
		if (!current_controller || current_controller == player_address)
			continue;

		auto controller_pawn_handle = mem_manager.read_memory<uintptr_t>(current_controller + offsets.player_pawn_handle);
		if (!controller_pawn_handle)
			continue;

		auto player = read_player(entity_list_start_address, controller_pawn_handle, player_address);
		if (player)
			other_players.emplace_back(*player);
	}

	return other_players;
}

Movement GameInformationhandler::read_controlled_player_movement(uintptr_t player_address)
{
	Movement return_value = {};
	return_value.forward = mem_manager.read_memory<DWORD>(client_dll_address + offsets.force_forward) == button_pressed_value;
	return_value.backward = mem_manager.read_memory<DWORD>(client_dll_address + offsets.force_backward) == button_pressed_value;
	return_value.left = mem_manager.read_memory<DWORD>(client_dll_address + offsets.force_left) == button_pressed_value;
	return_value.right = mem_manager.read_memory<DWORD>(client_dll_address + offsets.force_right) == button_pressed_value;

	return return_value;
}

Vec3D<float> GameInformationhandler::get_head_bone_position(uintptr_t player_pawn)
{
	constexpr DWORD bone_matrix_offset = 0x80;
	constexpr DWORD head_bone_index = 0x6;
	constexpr DWORD matrix_size = 0x20;

	auto game_scene_node = mem_manager.read_memory<uintptr_t>(player_pawn + offsets.sceneNode);
	auto bone_matrix = mem_manager.read_memory<uintptr_t>(game_scene_node + offsets.model_state + bone_matrix_offset);
	auto bone = mem_manager.read_memory<Vec3D<float>>(bone_matrix + (head_bone_index * matrix_size));

	return bone;
}

uintptr_t GameInformationhandler::get_list_entity(uintptr_t id, uintptr_t entity_list)
{
	// Bit magic to get the list entity, even if a pawn handle was given instead of an ID
	return mem_manager.read_memory<uintptr_t>(entity_list + ((8 * ((id & 0x7FFF) >> 9)) + 0x10));
}

uintptr_t GameInformationhandler::get_entity_controller_or_pawn(uintptr_t list_entity, uintptr_t id)
{
	// Bit magic to get the controller or pawn, even if a pointer was given instead of an ID
	return mem_manager.read_memory<uintptr_t>(list_entity + (id & 0x1FF) * 0x78);
}

std::optional<PlayerInformation> GameInformationhandler::read_player(uintptr_t entity_list_begin, uintptr_t id, uintptr_t player_address)
{
	uintptr_t listEntity = get_list_entity(id, entity_list_begin);
	if (!listEntity)
		return {};

	auto current_controller = get_entity_controller_or_pawn(listEntity, id);
	if (!current_controller || current_controller == player_address)
		return {};

	PlayerInformation ent;
	ent.position = mem_manager.read_memory<Vec3D<float>>(current_controller + offsets.position);
	ent.health = mem_manager.read_memory<DWORD>(current_controller + offsets.player_health_offset);
	ent.team = mem_manager.read_memory<int>(current_controller + offsets.team_offset);
	ent.head_position = get_head_bone_position(current_controller);
	return ent;
}

std::optional<PlayerInformation> GameInformationhandler::read_player_in_crosshair(uintptr_t player_controller, uintptr_t player_pawn)
{
	const auto cross_hair_ID = mem_manager.read_memory<int>(player_pawn + offsets.crosshair_offset);

	if (cross_hair_ID <= 0)
		return {};

	uintptr_t entity_list_start_address = mem_manager.read_memory<uintptr_t>(client_dll_address + offsets.entity_list_start_offset);
	if (!entity_list_start_address)
		return {};

	return read_player(entity_list_start_address, cross_hair_ID, player_controller);
}
