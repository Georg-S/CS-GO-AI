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
}

GameInformation GameInformationhandler::get_game_information() const
{
    return game_information;
}

bool GameInformationhandler::read_controlled_player_information(ControlledPlayer& dest, DWORD player_address, DWORD engine_client_state_address)
{
    dest.view_vec = mem_manager.read_memory<Vec2D<float>>(engine_client_state_address + Offsets::client_state_view_angle);
    dest.health = mem_manager.read_memory<int>(player_address + Offsets::player_health_offset);
    dest.team = mem_manager.read_memory<int>(player_address + Offsets::team_offset);
    dest.head_position = get_head_bone_position(player_address);
}

void GameInformationhandler::update_other_players(DWORD player_address, DWORD engine_client_state_address)
{
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
