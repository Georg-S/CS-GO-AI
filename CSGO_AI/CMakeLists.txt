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

set (UTILITY_HEADER
	"include/Utility/json.hpp"
	"include/Utility/Logger.h"
	"include/Utility/Logging.h"
	"include/Utility/Vec2D.h"
	"include/Utility/Vec3D.h"
	"include/Utility/Utility.h"
	"include/Utility/Dijkstra.h"
)

set (UI_HEADER
	"include/UI/CSGORunner.h"
	"include/UI/MainWindow.h"
	"include/UI/NavmeshEditor.h"
	"include/UI/NavmeshEditorWidget.h"
	"include/UI/QTBoxLogger.h"
)

set(QT_UI_FILES
	"include/UI/MainWindow.ui"
	"include/UI/NavmeshEditorWidget.ui"
)

set (CSGO_HEADER
	"include/CSGO/Aimbot.h"
	"include/CSGO/Button.h"
	"include/CSGO/ConfigReader.h"
	"include/CSGO/CSGOAI.h"
	"include/CSGO/GameInformationHandler.h"
	"include/CSGO/MemoryManager.h"
	"include/CSGO/MovementStrategy.h"
	"include/CSGO/NavmeshPoints.h"
	"include/CSGO/Offsets.h"
	"include/CSGO/Triggerbot.h"
)

set (HEADERS
	${UTILITY_HEADER}
	${UI_HEADER}
	${CSGO_HEADER}
	)

set(CSGO_SOURCE
	"src/CSGO/Aimbot.cpp"
	"src/CSGO/Button.cpp"
	"src/CSGO/ConfigReader.cpp"
	"src/CSGO/CSGOAI.cpp"
	"src/CSGO/GameInformationHandler.cpp"
	"src/CSGO/MemoryManager.cpp"
	"src/CSGO/MovementStrategy.cpp"
	"src/CSGO/NavmeshPoints.cpp"
	"src/CSGO/Offsets.cpp"
	"src/CSGO/Triggerbot.cpp"
)

set(UI_SOURCE
	"src/UI/CSGORunner.cpp"
	"src/UI/MainWindow.cpp"
	"src/UI/NavmeshEditor.cpp"
	"src/UI/NavmeshEditorWidget.cpp"
	"src/UI/QTBoxLogger.cpp"
)

set(UTILITY_SOURCE
	"src/Utility/Logging.cpp"
	"src/Utility/Utility.cpp"
	"src/Utility/Dijkstra.cpp"
)

set(SOURCES 
	${CSGO_SOURCE}
	${UI_SOURCE}
	${UTILITY_SOURCE}
	)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

source_group("\\Header Files\\Utility" FILES ${UTILITY_HEADER})
source_group("\\Header Files\\UI" FILES ${UI_HEADER} ${QT_UI_FILES})
source_group("\\Header Files\\CSGO" FILES ${CSGO_HEADER})

source_group("\\Source Files\\Utility" FILES ${UTILITY_SOURCE})
source_group("\\Source Files\\UI" FILES ${UI_SOURCE})
source_group("\\Source Files\\CSGO" FILES ${CSGO_SOURCE})


#add_executable(CSGO_AI WIN32 "src/main.cpp" ${SOURCES} ${HEADERS} ${QT_UI_FILES})
add_executable(CSGO_AI "src/main.cpp" ${SOURCES} ${HEADERS} ${QT_UI_FILES})
target_include_directories(CSGO_AI PUBLIC "include")
set_property(TARGET CSGO_AI PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT CSGO_AI)
target_link_libraries(CSGO_AI Qt5::Widgets)

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
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}"/Navmesh"
					${CMAKE_CURRENT_BINARY_DIR}"/Release/Navmesh")	

add_custom_command(TARGET CSGO_AI 
					POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}"/Navmesh"
					${CMAKE_CURRENT_BINARY_DIR}"/Debug/Navmesh")
