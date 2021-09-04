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

struct ActivatedFeatures 
{
	bool aimbot = false;
	bool movement = false;
	bool triggerBot = false;
};

class CSGOAi 
{
public:
	CSGOAi();
	bool init();
	void update();
	void run();
	bool load_config(const std::string& file_name);
	bool load_offsets(const std::string& file_name);
	bool load_navmesh(const std::string& file_name);
	bool attach_to_csgo_process();
	void set_activated_behavior(const ActivatedFeatures& behavior);

private:
	bool attached_to_process = false;
	ConfigData config;
	std::unique_ptr<GameInformationhandler> game_info_handler = nullptr;
	Triggerbot triggerbot;
	Aimbot aimbot;
	MovementStrategy movement_strategy;
	Button toggle_button;
	ActivatedFeatures activated_behavior;
};