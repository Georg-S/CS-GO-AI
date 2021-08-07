cmake_minimum_required (VERSION 3.8)
project(CSGO_AI)
project(NavmeshPointsaver)
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
	"include/Triggerbot.h"
	"include/Aimbot.h"
	"include/MovementStrategy.h"
	"include/NavmeshPoints.h"
	)

set(SOURCES 
	"src/MemoryManager.cpp"
	"src/FileReader.cpp"
	"src/Button.cpp"
	"src/Offsets.cpp"
	"src/GameInformationHandler.cpp"
	"src/ConfigReader.cpp"
	"src/CSGOAI.cpp"
	"src/Triggerbot.cpp"
	"src/Aimbot.cpp"
	"src/MovementStrategy.cpp"
	"src/NavmeshPoints.cpp"
	)

# CS-GO AI Project
add_executable(CSGO_AI "src/main.cpp" ${SOURCES} ${HEADERS})
target_include_directories(CSGO_AI PUBLIC "include")
set_property(TARGET CSGO_AI PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT CSGO_AI)

#Navmeshpointcreator Project
add_executable(NavmeshPointsaver "src/main_navmeshpointcreator.cpp" ${SOURCES} ${HEADERS})
target_include_directories(NavmeshPointsaver PUBLIC "include")
set_property(TARGET NavmeshPointsaver PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

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

add_custom_command(TARGET CSGO_AI 
					POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy 
					${CMAKE_SOURCE_DIR}"/nav_mesh.json"
					${CMAKE_CURRENT_BINARY_DIR}"/Debug/nav_mesh.json")

add_custom_command(TARGET CSGO_AI 
					POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy 
					${CMAKE_SOURCE_DIR}"/nav_mesh.json"
					${CMAKE_CURRENT_BINARY_DIR}"/Release/nav_mesh.json")

add_custom_command(TARGET NavmeshPointsaver 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy 
				   ${CMAKE_SOURCE_DIR}"/config.txt"
				   ${CMAKE_CURRENT_BINARY_DIR}"/Debug/config.txt")

add_custom_command(TARGET NavmeshPointsaver 
					POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy 
					${CMAKE_SOURCE_DIR}"/config.txt"
					${CMAKE_CURRENT_BINARY_DIR}"/Release/config.txt")

add_custom_command(TARGET NavmeshPointsaver 
					POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy 
					${CMAKE_SOURCE_DIR}"/offsets.json"
					${CMAKE_CURRENT_BINARY_DIR}"/Debug/offsets.json")

add_custom_command(TARGET NavmeshPointsaver 
					POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy 
					${CMAKE_SOURCE_DIR}"/offsets.json"
					${CMAKE_CURRENT_BINARY_DIR}"/Release/offsets.json")