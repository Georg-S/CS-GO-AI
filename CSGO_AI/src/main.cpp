#pragma once
#include "CSGOAI.h"


int main()
{
	CSGOAi ai{};

	if (ai.init())
		ai.run();

	return 0;
}
