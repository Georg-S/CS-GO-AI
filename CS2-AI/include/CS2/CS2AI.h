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
#include "Utility/Logging.h"

struct ActivatedFeatures 
{
	bool aimbot = false;
	bool movement = false;
	bool triggerBot = false;
};

class CS2Ai 
{
public:
	CS2Ai();
	void update();
	bool load_config(const std::string& file_name);
	bool load_offsets(const std::string& file_name);
	bool load_navmesh();
	bool attach_to_csgo_process();
	void set_activated_behavior(const ActivatedFeatures& behavior);
	std::shared_ptr<GameInformationhandler> get_game_info_handler() const;

private:
	Config config;
	std::shared_ptr<GameInformationhandler> game_info_handler = nullptr;
	MovementStrategy movement_strategy;
	Triggerbot triggerbot;
	Aimbot aimbot;
	Button toggle_button;
	ActivatedFeatures activated_behavior;
};