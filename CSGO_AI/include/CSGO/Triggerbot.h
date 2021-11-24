#pragma once
#include <chrono>
#include "GameInformationHandler.h"

class Triggerbot 
{
public:
	void update(GameInformationhandler* handler);
private:
	long long delay_time = 0;
};