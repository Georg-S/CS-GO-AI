#include "CSGO/GameInformationHandler.h"

bool GameInformationhandler::init(const Config& config)
{
    mem_manager.attach_to_process(config.windowname.c_str());

    client_dll_address = mem_manager.get_module_address(config.client_dll_name.c_str());
    engine_address = mem_manager.get_module_address(config.engine_dll_name.c_str());

    attached_to_process = (client_dll_address != NULL) && (engine_address != NULL);

    return attached_to_process;
}

bool GameInformationhandler::loadOffsets(const std::string& file_name)
{
    auto offsets = load_offsets_from_file(file_name);
    if (offsets)
        this->offsets = offsets.value();

    return offsets.has_value();
}

void GameInformationhandler::update_game_information()
{
    DWORD player_address = mem_manager.read_memory<DWORD>(client_dll_address + offsets.local_player_offset);
    DWORD engine_client_state = mem_manager.read_memory<DWORD>(engine_address + offsets.client_state);

    game_information.controlled_player = read_controlled_player_information(player_address, engine_client_state);
    game_information.player_in_crosshair = read_player_in_crosshair(player_address);
    game_information.other_players = read_other_players(player_address, engine_client_state);
    game_information.closest_enemy_player = get_closest_enemy(game_information);
    read_in_current_map(engine_client_state, game_information.current_map, 64);
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

    DWORD engine_client_state_address = mem_manager.read_memory<DWORD>(engine_address + offsets.client_state);

    mem_manager.write_memory<Vec2D<float>>(engine_client_state_address + offsets.client_state_view_angle, view_vec);
}

void GameInformationhandler::set_player_movement(const Movement& movement)
{
    mem_manager.write_memory<bool>(client_dll_address + offsets.force_forward, movement.forward);
    mem_manager.write_memory<bool>(client_dll_address + offsets.force_backward, movement.backward);
    mem_manager.write_memory<bool>(client_dll_address + offsets.force_left, movement.left);
    mem_manager.write_memory<bool>(client_dll_address + offsets.force_right, movement.right);
}

void GameInformationhandler::set_player_shooting(bool val)
{
    constexpr DWORD shooting_value = 5;
    constexpr DWORD not_shooting_value = 4;

    DWORD mem_val = val ? shooting_value : not_shooting_value;

    mem_manager.write_memory<DWORD>(client_dll_address + offsets.force_attack, mem_val);
}

std::shared_ptr<PlayerInformation> GameInformationhandler::get_closest_enemy(const GameInformation& game_info)
{
    std::shared_ptr<PlayerInformation> closest_enemy = nullptr;
    const auto& controlled_player = game_info.controlled_player;
    float closest_distance = FLT_MAX;

    for (const auto& enemy : game_info.other_players)
    {
        float distance = controlled_player.head_position.distance(enemy.head_position);

        if ((distance <= closest_distance) && (enemy.team != controlled_player.team) && (enemy.health > 0))
        {
            closest_distance = distance;
            closest_enemy = std::make_shared<PlayerInformation>(enemy);
        }
    }

    return closest_enemy;
}

void GameInformationhandler::read_in_current_map(DWORD engine_client_state_address, char* buffer, DWORD buffer_size)
{
    mem_manager.read_string_from_memory(engine_client_state_address + offsets.current_map, buffer, buffer_size);
}

bool GameInformationhandler::read_in_if_controlled_player_is_shooting()
{
    constexpr DWORD shooting_value = 5;
    DWORD val = mem_manager.read_memory<DWORD>(client_dll_address + offsets.force_attack);

    return val == shooting_value;
}

ControlledPlayer GameInformationhandler::read_controlled_player_information(DWORD player_address, DWORD engine_client_state_address)
{
    ControlledPlayer dest{};
    dest.view_vec = mem_manager.read_memory<Vec2D<float>>(engine_client_state_address + offsets.client_state_view_angle);
    dest.health = mem_manager.read_memory<int>(player_address + offsets.player_health_offset);
    dest.team = mem_manager.read_memory<int>(player_address + offsets.team_offset);
    dest.position = mem_manager.read_memory<Vec3D<float>>(player_address + offsets.position);
    dest.shots_fired = mem_manager.read_memory<DWORD>(player_address + offsets.shots_fired_offset);
    dest.shooting = read_in_if_controlled_player_is_shooting();
    dest.head_position = get_head_bone_position(player_address);
    dest.movement = read_controlled_player_movement(player_address);

    return dest;
}

std::vector<PlayerInformation> GameInformationhandler::read_other_players(DWORD player_address, DWORD engine_client_state_address)
{
    int max_players = mem_manager.read_memory<int>(engine_client_state_address + offsets.client_state_max_players);
    std::vector<PlayerInformation> other_players;

    for (int i = 0; i < max_players; i++)
    {
        DWORD entity_address = mem_manager.read_memory<DWORD>(client_dll_address + offsets.entity_list_start_offset
            + offsets.entity_listelement_size * i);

        if (!entity_address || entity_address == player_address)
            continue;

        PlayerInformation ent;
        ent.position = mem_manager.read_memory<Vec3D<float>>(entity_address + offsets.position);
        ent.head_position = get_head_bone_position(entity_address);
        ent.health = mem_manager.read_memory<DWORD>(entity_address + offsets.player_health_offset);
        ent.team = mem_manager.read_memory<int>(entity_address + offsets.team_offset);
        other_players.push_back(ent);
    }

    return other_players;
}

Movement GameInformationhandler::read_controlled_player_movement(DWORD player_address)
{
    Movement return_value;

    return_value.forward = mem_manager.read_memory<bool>(client_dll_address + offsets.force_forward);
    return_value.backward = mem_manager.read_memory<bool>(client_dll_address + offsets.force_backward);
    return_value.left = mem_manager.read_memory<bool>(client_dll_address + offsets.force_left);
    return_value.right = mem_manager.read_memory<bool>(client_dll_address + offsets.force_right);

    return return_value;
}

Vec3D<float> GameInformationhandler::get_head_bone_position(DWORD entity)
{
    //Bone matrix is a 3 row 4 column matrix  3 * 4 * 4 = hex 30
    constexpr DWORD head_bone_index = 0x8;
    constexpr DWORD matrix_size = 0x30;
    Vec3D<float> pos;

    DWORD bones_address = mem_manager.read_memory<DWORD>(entity + offsets.bone_matrix);
    //0C,1c,2c because we want the right column of the matrix
    pos.x = mem_manager.read_memory<float>(bones_address + matrix_size * head_bone_index + 0x0C);
    pos.y = mem_manager.read_memory<float>(bones_address + matrix_size * head_bone_index + 0x1C);
    pos.z = mem_manager.read_memory<float>(bones_address + matrix_size * head_bone_index + 0x2C);

    return pos;
}

std::shared_ptr<PlayerInformation> GameInformationhandler::read_player_in_crosshair(DWORD player_address)
{
    int cross_hair_ID = mem_manager.read_memory<int>(player_address + offsets.crosshair_offset);

    if (cross_hair_ID <= 0 || cross_hair_ID > 100)
        return nullptr;

    auto player_info = std::make_shared<PlayerInformation>();

    DWORD enemy_in_crosshair_address = mem_manager.read_memory<DWORD>(
        client_dll_address + offsets.entity_list_start_offset + ((cross_hair_ID - 1) * offsets.entity_listelement_size));
    player_info->health = mem_manager.read_memory<int>(enemy_in_crosshair_address + offsets.player_health_offset);
    player_info->team = mem_manager.read_memory<int>(enemy_in_crosshair_address + offsets.team_offset);
    player_info->position = mem_manager.read_memory<Vec3D<float>>(enemy_in_crosshair_address + offsets.position);
    player_info->head_position = get_head_bone_position(enemy_in_crosshair_address);

    return player_info;
}
