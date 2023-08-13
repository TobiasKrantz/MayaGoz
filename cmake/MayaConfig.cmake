add_library(Python::Lib SHARED IMPORTED)
add_library(Maya::Foundation SHARED IMPORTED)
add_library(Maya::OpenMaya SHARED IMPORTED)
add_library(Maya::OpenMayaRender SHARED IMPORTED)
add_library(Maya::OpenMayaUI SHARED IMPORTED)
add_library(Maya::OpenMayaAnim SHARED IMPORTED)
add_library(Maya::OpenMayaFX SHARED IMPORTED)

set(MAYA_INCLUDE_DIR ${MAYA_DEVKIT_ROOT}/include)
set(MAYA_LIB_DIR     ${MAYA_DEVKIT_ROOT}/lib)

set_target_properties(Python::Lib PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${MAYA_INCLUDE_DIR}/Python27/Python
    IMPORTED_IMPLIB ${MAYA_LIB_DIR}/python27.lib
)

set_target_properties(Maya::Foundation PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${MAYA_INCLUDE_DIR}
    IMPORTED_IMPLIB ${MAYA_LIB_DIR}/Foundation.lib
)

set_target_properties(Maya::OpenMaya PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${MAYA_INCLUDE_DIR}
    IMPORTED_IMPLIB ${MAYA_LIB_DIR}/OpenMaya.lib
)

set_target_properties(Maya::OpenMayaRender PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${MAYA_INCLUDE_DIR}
    IMPORTED_IMPLIB ${MAYA_LIB_DIR}/OpenMayaRender.lib
)

set_target_properties(Maya::OpenMayaUI PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${MAYA_INCLUDE_DIR}
    IMPORTED_IMPLIB ${MAYA_LIB_DIR}/OpenMayaUI.lib
)

set_target_properties(Maya::OpenMayaAnim PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${MAYA_INCLUDE_DIR}
    IMPORTED_IMPLIB ${MAYA_LIB_DIR}/OpenMayaAnim.lib
)

set_target_properties(Maya::OpenMayaFX PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${MAYA_INCLUDE_DIR}
    IMPORTED_IMPLIB ${MAYA_LIB_DIR}/OpenMayaFX.lib
)
