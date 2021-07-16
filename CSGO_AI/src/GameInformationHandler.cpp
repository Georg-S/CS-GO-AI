#include "GameInformationHandler.h"

GameInformationhandler::GameInformationhandler()
{
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
    return this->game_information;
}
