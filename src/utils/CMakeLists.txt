add_library(${CMAKE_PROJECT_NAME}Utils
    SHARED
    ${CMAKE_SOURCE_DIR}/include/internal/utils/logger.h
    logger.cpp
    internal/logger.hpp
    internal/logger.cpp
    ${CMAKE_SOURCE_DIR}/include/internal/utils/math.h
    math.c
    ${CMAKE_SOURCE_DIR}/include/internal/utils/misc.h
    misc.c
    ${CMAKE_SOURCE_DIR}/include/internal/utils/ptrvector.h
    ptrvector.c
    ${CMAKE_SOURCE_DIR}/include/internal/utils/utf8.h
    utf8.cpp
    ${CMAKE_SOURCE_DIR}/include/internal/utils.h
    ${CMAKE_SOURCE_DIR}/include/SubFX.h
)

add_library(${CMAKE_PROJECT_NAME}::Utils ALIAS ${CMAKE_PROJECT_NAME}Utils)
file(WRITE
    ${CMAKE_BINARY_DIR}/utils/${CMAKE_PROJECT_NAME}UtilsConfig.cmake
    "")

target_include_directories(${CMAKE_PROJECT_NAME}Utils
    SYSTEM BEFORE
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
)

set_target_properties(${CMAKE_PROJECT_NAME}Utils
    PROPERTIES

    EXPORT_NAME Utils

    VERSION ${PROJECT_VERSION}

    PUBLIC_HEADER
    "${CMAKE_SOURCE_DIR}/include/SubFX.h"

    PRIVATE_HEADER
    "${CMAKE_SOURCE_DIR}/include/internal/utils/logger.h;\
${CMAKE_SOURCE_DIR}/include/internal/utils/math.h;\
${CMAKE_SOURCE_DIR}/include/internal/utils/misc.h;\
${CMAKE_SOURCE_DIR}/include/internal/utils/utf8.h;\
${CMAKE_SOURCE_DIR}/include/internal/utils.h"
)

install(
    TARGETS ${CMAKE_PROJECT_NAME}Utils
    DESTINATION
    ${CMAKE_INSTALL_LIBDIR}

    EXPORT ${CMAKE_PROJECT_NAME}Targets
    DESTINATION
    ${CMAKE_INSTALL_LIBDIR}

    RUNTIME
    DESTINATION
    ${CMAKE_INSTALL_BINDIR}
    COMPONENT
    runtime

    ARCHIVE
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT
    devel

    PUBLIC_HEADER
    DESTINATION
    ${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_PROJECT_NAME}
    COMPONENT
    devel

    PRIVATE_HEADER
    DESTINATION
    ${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_PROJECT_NAME}/internal/utils
    COMPONENT
    devel
)
