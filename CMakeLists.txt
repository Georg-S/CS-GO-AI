cmake_minimum_required (VERSION 3.8)
project(CS-GO-AI)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(NavmeshPointsaver)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT NavmeshPointsaver)