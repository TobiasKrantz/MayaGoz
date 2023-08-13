cmake_minimum_required (VERSION 3.6)
cmake_policy(SET CMP0048 NEW)

project(GozPlugin)

set(MAYA_DEVKIT_ROOT $ENV{DEVKIT_LOCATION})
set(CMAKE_MODULE_PATH ${MAYA_DEVKIT_ROOT}/cmake/modules)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(ENTRY ${SOURCE_DIR}/maya_plugin_main.cpp)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/MayaConfig.cmake) # From ./cmake

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${SOURCE_DIR} ${MAYA_INCLUDE_DIR}) 

add_subdirectory(src)

add_library(${PROJECT_NAME} SHARED ${ENTRY})

target_link_libraries(${PROJECT_NAME}
    goz
    goz_maya
    Maya::Foundation
    Maya::OpenMaya 
)
 
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".mll")

