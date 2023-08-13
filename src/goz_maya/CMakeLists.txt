project(goz_maya)

file(GLOB SOURCES *.cpp *.h)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    goz
    Maya::Foundation
    Maya::OpenMaya 
)
 
