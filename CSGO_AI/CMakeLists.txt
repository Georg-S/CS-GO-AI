﻿cmake_minimum_required (VERSION 3.8)
project(CSGO_AI)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)


set (HEADERS
	"include/MemoryManager.h"
	"include/FileReader.h"
	"include/Button.h"
	"include/Offsets.h"
	"include/Vec3D.h"
	"include/Vec2D.h"
	"include/json.hpp"
	"include/GameInformationHandler.h"
	"include/ConfigReader.h"
	"include/CSGOAI.h"
	)

set(SOURCES 
	"src/main.cpp"
	"src/MemoryManager.cpp"
	"src/FileReader.cpp"
	"src/Button.cpp"
	"src/Offsets.cpp"
	"src/GameInformationHandler.cpp"
	"src/ConfigReader.cpp"
	"src/CSGOAI.cpp"
	)

add_executable(CSGO_AI ${SOURCES} ${HEADERS})
target_include_directories(CSGO_AI PUBLIC "include")
set_property(TARGET CSGO_AI PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT CSGO_AI)

add_custom_command(TARGET CSGO_AI 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy 
				   ${CMAKE_SOURCE_DIR}"/config.txt"
				   ${CMAKE_CURRENT_BINARY_DIR}"/Debug/config.txt")

add_custom_command(TARGET CSGO_AI 
					POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy 
					${CMAKE_SOURCE_DIR}"/config.txt"
					${CMAKE_CURRENT_BINARY_DIR}"/Release/config.txt")

add_custom_command(TARGET CSGO_AI 
					POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy 
					${CMAKE_SOURCE_DIR}"/offsets.json"
					${CMAKE_CURRENT_BINARY_DIR}"/Debug/offsets.json")

add_custom_command(TARGET CSGO_AI 
					POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy 
					${CMAKE_SOURCE_DIR}"/offsets.json"
					${CMAKE_CURRENT_BINARY_DIR}"/Release/offsets.json")