#pragma once
#include "NavmeshPoints.h"

int main()
{
	NavmeshPoints program = NavmeshPoints();

	if (program.init())
		program.run();

	return 0;
}
