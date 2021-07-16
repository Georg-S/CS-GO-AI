cmake_minimum_required (VERSION 3.8)
project(CSGO_AI)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set (HEADERS
	)

set(SOURCES 
	"src/main.cpp"
	)


add_executable(CSGO_AI ${SOURCES} ${HEADERS})
target_include_directories(CSGO_AI PUBLIC "include")
set_property(TARGET CSGO_AI PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT CSGO_AI)