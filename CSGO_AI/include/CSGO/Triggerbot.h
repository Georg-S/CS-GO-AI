#pragma once
#include <chrono>
#include "GameInformationHandler.h"

class Triggerbot 
{
public:
	void update(GameInformationhandler* handler);
	void simulate_left_click();
private:
	long long delay_time = 0;
};