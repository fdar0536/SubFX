add_library(SubFXUtils
    SHARED
    include/internal/utils.hpp
    include/internal/utils/logger.hpp
    utils/logger.cpp
    include/Utils
)

target_include_directories(SubFXUtils
    SYSTEM BEFORE
    PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

set_target_properties(SubFXUtils
    PROPERTIES
    PUBLIC_HEADER
    "include/Utils"
)

set_target_properties(SubFXUtils
    PROPERTIES
    PRIVATE_HEADER
    "include/internal/utils/logger.hpp;\
include/internal/utils.hpp"
)

install(
    TARGETS SubFXUtils

    LIBRARY
    DESTINATION
    ${CMAKE_INSTALL_LIBDIR}

    PUBLIC_HEADER
    DESTINATION
    ${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_PROJECT_NAME}/internal

    PRIVATE_HEADER
    DESTINATION
    ${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_PROJECT_NAME}/internal/utils
)
