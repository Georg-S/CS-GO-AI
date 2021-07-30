#pragma once
#include "GameInformationHandler.h"
#include <chrono>

class Triggerbot 
{
public:
	void update(GameInformationhandler* handler);
	void simulate_left_click();
private:
	long long delay_time = 0;
};