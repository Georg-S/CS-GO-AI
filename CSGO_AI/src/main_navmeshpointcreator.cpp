#pragma once
#include "NavmeshPoints.h"

int main()
{
	NavmeshPoints program = NavmeshPoints();

	if (program.init())
		program.console_run();

	return 0;
}
