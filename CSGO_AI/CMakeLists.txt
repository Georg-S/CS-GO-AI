cmake_minimum_required (VERSION 3.8)
project(CSGO_AI)
project(NavmeshPointsaver)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
   list(APPEND CMAKE_PREFIX_PATH $ENV{QT_64_MSVC2017})
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
   list(APPEND CMAKE_PREFIX_PATH $ENV{QT_x86_MSVC2017})
endif()

find_package(Qt5Widgets REQUIRED)

set (HEADERS
	"include/MemoryManager.h"
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
	"include/MainWindow.h"
	"include/CSGORunner.h"
	"include/Logger.h"
	"include/QTBoxLogger.h"
	"include/Logging.h"
	"include/NavmeshEditorWidget.h"
	"include/NavmeshEditor.h"
	)

set(SOURCES 
	"src/MemoryManager.cpp"
	"src/Button.cpp"
	"src/Offsets.cpp"
	"src/GameInformationHandler.cpp"
	"src/ConfigReader.cpp"
	"src/CSGOAI.cpp"
	"src/Triggerbot.cpp"
	"src/Aimbot.cpp"
	"src/MovementStrategy.cpp"
	"src/NavmeshPoints.cpp"
	"src/MainWindow.cpp"
	"src/CSGORunner.cpp"
	"src/QTBoxLogger.cpp"
	"src/Logging.cpp"
	"src/NavmeshEditor.cpp"
	"src/NavmeshEditorWidget.cpp"
	)

set(QT_UI_FILES
	"include/MainWindow.ui"
	"include/NavmeshEditorWidget.ui"
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# CS-GO AI Project
#add_executable(CSGO_AI WIN32 "src/main.cpp" ${SOURCES} ${HEADERS} ${QT_UI_FILES})
add_executable(CSGO_AI "src/main.cpp" ${SOURCES} ${HEADERS} ${QT_UI_FILES})
target_include_directories(CSGO_AI PUBLIC "include")
set_property(TARGET CSGO_AI PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT CSGO_AI)
target_link_libraries(CSGO_AI Qt5::Widgets)

#[[
#Navmeshpointcreator Project
add_executable(NavmeshPointsaver "src/main_navmeshpointcreator.cpp" ${SOURCES} ${HEADERS} ${QT_UI_FILES})
target_include_directories(NavmeshPointsaver PUBLIC "include")
set_property(TARGET NavmeshPointsaver PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
target_link_libraries(NavmeshPointsaver Qt5::Widgets)
]]


add_custom_command(TARGET CSGO_AI 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy 
				   ${CMAKE_SOURCE_DIR}"/config.json"
				   ${CMAKE_CURRENT_BINARY_DIR}"/Debug/config.json")

add_custom_command(TARGET CSGO_AI 
					POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy 
					${CMAKE_SOURCE_DIR}"/config.json"
					${CMAKE_CURRENT_BINARY_DIR}"/Release/config.json")

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

#[[
add_custom_command(TARGET NavmeshPointsaver 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy 
				   ${CMAKE_SOURCE_DIR}"/config.json"
				   ${CMAKE_CURRENT_BINARY_DIR}"/Debug/config.json")

add_custom_command(TARGET NavmeshPointsaver 
					POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy 
					${CMAKE_SOURCE_DIR}"/config.json"
					${CMAKE_CURRENT_BINARY_DIR}"/Release/config.json")

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
]]