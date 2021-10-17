#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include "Offsets.h"
#include "ConfigReader.h"
#include "GameInformationHandler.h"
#include "Triggerbot.h"
#include "Aimbot.h"
#include "Button.h"
#include "MovementStrategy.h"
#include "Logger.h"

struct ActivatedFeatures 
{
	bool aimbot = false;
	bool movement = false;
	bool triggerBot = false;
};

class CSGOAi 
{
public:
	CSGOAi(std::shared_ptr<Logger> logger);
	bool init();
	void update();
	void console_run();
	bool load_config(const std::string& file_name);
	bool load_offsets(const std::string& file_name);
	bool load_navmesh(const std::string& file_name);
	bool attach_to_csgo_process();
	void set_activated_behavior(const ActivatedFeatures& behavior);
	std::shared_ptr<GameInformationhandler> get_game_info_handler();
	std::shared_ptr<Logger> get_logger();

private:
	ConfigData config;
	std::shared_ptr<GameInformationhandler> game_info_handler = nullptr;
	std::shared_ptr<Logger> logger = nullptr;;
	std::unique_ptr<MovementStrategy> movement_strategy = nullptr;
	Triggerbot triggerbot;
	Aimbot aimbot;
	Button toggle_button;
	ActivatedFeatures activated_behavior;
};