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

class CSGOAi 
{
public:
	CSGOAi();
	bool init();
	void run();

private:
	ConfigData config;
	std::unique_ptr<GameInformationhandler> game_info_handler = nullptr;
	Triggerbot triggerbot;
	Aimbot aimbot;
	MovementStrategy movement_strategy;
	Button toggle_button;
};