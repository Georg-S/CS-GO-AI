#pragma once
#include "Utility/Utility.h"
#include "GameInformationHandler.h"

class Triggerbot 
{
public:
	void update(GameInformationhandler* handler);
private:
	long long m_delay_time = 0;
};