#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include "Offsets.h"
#include "ConfigReader.h"
#include "GameInformationHandler.h"

class CSGOAi 
{
public:
	CSGOAi();
	bool init();


private:
	std::unique_ptr<GameInformationhandler> gameInfoHandler = nullptr;
};