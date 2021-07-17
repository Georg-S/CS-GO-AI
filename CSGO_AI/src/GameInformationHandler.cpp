#include "GameInformationHandler.h"

GameInformationhandler::GameInformationhandler()
{
    client_dll_address = 0;
    engine_address = 0;
}

bool GameInformationhandler::init(const ConfigData& config)
{
    client_dll_address = mem_manager.get_module_address(config.client_dll_name.c_str());
    engine_address = mem_manager.get_module_address(config.engine_dll_name.c_str());

    return (client_dll_address != NULL) && (engine_address != NULL);
}

void GameInformationhandler::update_game_information()
{
    DWORD player_address = mem_manager.read_memory<DWORD>(client_dll_address + Offsets::local_player_offset);
    DWORD engine_client_state = mem_manager.read_memory<DWORD>(engine_address + Offsets::clientState);

    game_information.controlled_player = read_controlled_player_information(player_address, engine_client_state);
    game_information.player_in_crosshair = read_player_in_crosshair(player_address);
    game_information.other_players = read_other_players(player_address, engine_client_state);
}

GameInformation GameInformationhandler::get_game_information() const
{
    return game_information;
}

void GameInformationhandler::set_view_vec(const Vec2D<float>& view_vec)
{
    DWORD engine_client_state_address = mem_manager.read_memory<DWORD>(engine_address + Offsets::clientState);

    mem_manager.write_memory<Vec2D<float>>(engine_client_state_address + Offsets::client_state_view_angle, view_vec);
}

ControlledPlayer GameInformationhandler::read_controlled_player_information(DWORD player_address, DWORD engine_client_state_address)
{
    ControlledPlayer dest{};
    dest.view_vec = mem_manager.read_memory<Vec2D<float>>(engine_client_state_address + Offsets::client_state_view_angle);
    dest.health = mem_manager.read_memory<int>(player_address + Offsets::player_health_offset);
    dest.team = mem_manager.read_memory<int>(player_address + Offsets::team_offset);
    dest.head_position = get_head_bone_position(player_address);

    return dest;
}

std::vector<PlayerInformation> GameInformationhandler::read_other_players(DWORD player_address, DWORD engine_client_state_address)
{
    int max_players = mem_manager.read_memory<int>(engine_client_state_address + Offsets::client_state_max_players);
    std::vector<PlayerInformation> other_players;

    for (int i = 0; i < max_players; i++)
    {
        DWORD entity_address = mem_manager.read_memory<DWORD>(client_dll_address + Offsets::entity_list_start_offset
            + Offsets::entity_listelement_size * i);

        if (!entity_address || entity_address == player_address)
            continue;

        PlayerInformation ent;
        ent.head_position = get_head_bone_position(entity_address);
        ent.health = mem_manager.read_memory<DWORD>(entity_address + Offsets::player_health_offset);
        ent.team = mem_manager.read_memory<int>(entity_address + Offsets::team_offset);
        other_players.push_back(ent);
    }

    return other_players;
}

Vec3D<float> GameInformationhandler::get_head_bone_position(DWORD entity)
{
    //Bone matrix is a 3 row 4 column matrix  3 * 4 * 4 = hex 30
    constexpr DWORD head_bone_index = 0x8;
    constexpr DWORD matrix_size = 0x30;
    Vec3D<float> pos;

    DWORD bones_address = mem_manager.read_memory<DWORD>(entity + Offsets::bone_matrix);
    //0C,1c,2c because we want the right column of the matrix
    pos.x = mem_manager.read_memory<float>(bones_address + matrix_size * head_bone_index + 0x0C);
    pos.y = mem_manager.read_memory<float>(bones_address + matrix_size * head_bone_index + 0x1C);
    pos.z = mem_manager.read_memory<float>(bones_address + matrix_size * head_bone_index + 0x2C);

    return pos;
}

std::shared_ptr<PlayerInformation> GameInformationhandler::read_player_in_crosshair(DWORD player_address)
{
    int cross_hair_ID = mem_manager.read_memory<int>(player_address + Offsets::crosshair_offset);

    if (cross_hair_ID <= 0 || cross_hair_ID > 300)
        return nullptr;

    auto player_info = std::make_shared<PlayerInformation>();

    DWORD enemy_in_crosshair_address = mem_manager.read_memory<DWORD>(
        client_dll_address + Offsets::entity_list_start_offset + ((cross_hair_ID - 1) * Offsets::entity_listelement_size));
    player_info->health = mem_manager.read_memory<int>(enemy_in_crosshair_address + Offsets::player_health_offset);
    player_info->team = mem_manager.read_memory<int>(enemy_in_crosshair_address + Offsets::team_offset);
    player_info->head_position = get_head_bone_position(enemy_in_crosshair_address);

    return player_info;
}
