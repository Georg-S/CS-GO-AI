cmake_minimum_required (VERSION 3.8)
project(CS2_AI)
project(NavmeshPointsaver)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}")
include(windeployqt)

if	(MSVC)
    list(APPEND CMAKE_PREFIX_PATH $ENV{QT_64_MSVC2017})
endif(MSVC)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

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
	"include/UI/CS2Runner.h"
	"include/UI/MainWindow.h"
	"include/UI/NavmeshEditor.h"
	"include/UI/NavmeshEditorWidget.h"
	"include/UI/QTBoxLogger.h"
)

set(QT_UI_FILES
	"include/UI/MainWindow.ui"
	"include/UI/NavmeshEditorWidget.ui"
)

set (CS2_HEADER
	"include/CS2/Aimbot.h"
	"include/CS2/Button.h"
	"include/CS2/ConfigReader.h"
	"include/CS2/CS2AI.h"
	"include/CS2/GameInformationHandler.h"
	"include/CS2/MemoryManager.h"
	"include/CS2/MovementStrategy.h"
	"include/CS2/NavmeshPoints.h"
	"include/CS2/Offsets.h"
	"include/CS2/Triggerbot.h"
	"include/CS2/Constants.h"
)

set (HEADERS
	${UTILITY_HEADER}
	${UI_HEADER}
	${CS2_HEADER}
	)

set(CS2_SOURCE
	"src/CS2/Aimbot.cpp"
	"src/CS2/Button.cpp"
	"src/CS2/ConfigReader.cpp"
	"src/CS2/CS2AI.cpp"
	"src/CS2/GameInformationHandler.cpp"
	"src/CS2/MemoryManager.cpp"
	"src/CS2/MovementStrategy.cpp"
	"src/CS2/NavmeshPoints.cpp"
	"src/CS2/Offsets.cpp"
	"src/CS2/Triggerbot.cpp"
)

set(UI_SOURCE
	"src/UI/CS2Runner.cpp"
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
	${CS2_SOURCE}
	${UI_SOURCE}
	${UTILITY_SOURCE}
	)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

source_group("\\Header Files\\Utility" FILES ${UTILITY_HEADER})
source_group("\\Header Files\\UI" FILES ${UI_HEADER} ${QT_UI_FILES})
source_group("\\Header Files\\CS2" FILES ${CS2_HEADER})

source_group("\\Source Files\\Utility" FILES ${UTILITY_SOURCE})
source_group("\\Source Files\\UI" FILES ${UI_SOURCE})
source_group("\\Source Files\\CS2" FILES ${CS2_SOURCE})


add_executable(CS2_AI "src/main.cpp" ${SOURCES} ${HEADERS} ${QT_UI_FILES})
target_include_directories(CS2_AI PUBLIC "include")
set_property(TARGET CS2_AI PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT CS2_AI)
target_link_libraries(CS2_AI Qt6::Widgets)

if	(MSVC)
	target_compile_options(CS2_AI PRIVATE "/MP")
endif(MSVC)

add_custom_command(TARGET CS2_AI 
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory 
				   ${CMAKE_SOURCE_DIR}"/Configuration"
				   "$<TARGET_FILE_DIR:CS2_AI>/Configuration")

add_custom_command(TARGET CS2_AI 
					POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}"/Navmesh"
					"$<TARGET_FILE_DIR:CS2_AI>/Navmesh")

# TODO fix windeployqt
# windeployqt(CS2_AI)

