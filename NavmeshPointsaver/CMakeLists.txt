cmake_minimum_required (VERSION 3.8)
project(NavmeshPointsaver)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set (HEADERS
	"include/MemoryManager.h"
	"include/FileReader.h"
	"include/Button.h"
	"include/Offsets.h"
	"include/Vec3D.h"
	"include/Vec2D.h"
	"include/ConfigData.h"
	"include/NavmeshPoints.h"
	"include/json.hpp"
	)

set(SOURCES 
	"src/main.cpp"
	"src/MemoryManager.cpp"
	"src/FileReader.cpp"
	"src/Button.cpp"
	"src/Offsets.cpp"
	"src/NavmeshPoints.cpp"
	)


add_executable(NavmeshPointsaver ${SOURCES} ${HEADERS})
target_include_directories(NavmeshPointsaver PUBLIC "include")
set_property(TARGET NavmeshPointsaver PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

add_custom_command(TARGET NavmeshPointsaver 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy 
				   ${CMAKE_SOURCE_DIR}"/config.txt"
				   ${CMAKE_SOURCE_DIR}"/build/debug/config.txt")

add_custom_command(TARGET NavmeshPointsaver 
					POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy 
					${CMAKE_SOURCE_DIR}"/config.txt"
					${CMAKE_SOURCE_DIR}"/build/Release/config.txt")

add_custom_command(TARGET NavmeshPointsaver 
					POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy 
					${CMAKE_SOURCE_DIR}"/memoryOffsets.txt"
					${CMAKE_SOURCE_DIR}"/build/debug/memoryOffsets.txt")

add_custom_command(TARGET NavmeshPointsaver 
					POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy 
					${CMAKE_SOURCE_DIR}"/memoryOffsets.txt"
					${CMAKE_SOURCE_DIR}"/build/Release/memoryOffsets.txt")
