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
	bool load_config();
	bool load_offsets();
	bool load_navmesh();
	bool attach_to_cs2_process();
	void set_activated_behavior(const ActivatedFeatures& behavior);
	std::shared_ptr<GameInformationhandler> get_game_info_handler() const;

private:
	Config m_config;
	std::shared_ptr<GameInformationhandler> m_game_info_handler = nullptr;
	MovementStrategy m_movement_strategy;
	Triggerbot m_triggerbot;
	Aimbot m_aimbot;
	ActivatedFeatures m_activated_behavior;
};