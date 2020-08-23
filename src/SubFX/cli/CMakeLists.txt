add_executable(${CMAKE_PROJECT_NAME}CLI
    main.cpp
    asslauncher.hpp
    asslauncher.cpp
    ../common/configparser.hpp
    ../common/configparser.cpp
    ../common/print_script_template.hpp
)

set(${CMAKE_PROJECT_NAME}_YutilsPy_DIR "${CMAKE_BINARY_DIR}/YutilsCpp/python")
find_package(${CMAKE_PROJECT_NAME}_YutilsPy CONFIG REQUIRED)
add_dependencies(${CMAKE_PROJECT_NAME}CLI ${CMAKE_PROJECT_NAME}_YutilsPy)

set(${CMAKE_PROJECT_NAME}Utils_DIR "${CMAKE_BINARY_DIR}/utils")
find_package(${CMAKE_PROJECT_NAME}Utils CONFIG REQUIRED)
add_dependencies(${CMAKE_PROJECT_NAME}CLI ${CMAKE_PROJECT_NAME}Utils)

set(${CMAKE_PROJECT_NAME}YutilsCpp_DIR "${CMAKE_BINARY_DIR}/YutilsCpp")
find_package(${CMAKE_PROJECT_NAME}YutilsCpp CONFIG REQUIRED)
add_dependencies(${CMAKE_PROJECT_NAME}CLI ${CMAKE_PROJECT_NAME}YutilsCpp)

add_dependencies(${CMAKE_PROJECT_NAME}CLI BaseCommon)

find_package(cxxopts 2.2.0 REQUIRED)

LIST(APPEND ${CMAKE_PROJECT_NAME}CLI_includes ${nlohmann_json_INCLUDE_DIRS})
LIST(APPEND ${CMAKE_PROJECT_NAME}CLI_includes ${PYTHON_INCLUDE_DIRS})
LIST(APPEND ${CMAKE_PROJECT_NAME}CLI_includes ${pybind11_INCLUDE_DIRS})
LIST(APPEND ${CMAKE_PROJECT_NAME}CLI_includes ${cxxopts_INCLUDE_DIRS})
LIST(APPEND ${CMAKE_PROJECT_NAME}CLI_libs ${PYTHON_LIBRARIES})
LIST(APPEND ${CMAKE_PROJECT_NAME}CLI_libs ${CMAKE_PROJECT_NAME}::Utils)

target_link_libraries(SubFXCLI PRIVATE ${${CMAKE_PROJECT_NAME}CLI_libs})
target_include_directories(${CMAKE_PROJECT_NAME}CLI
    SYSTEM BEFORE
    PUBLIC
    ${${CMAKE_PROJECT_NAME}CLI_includes}
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)

# install
install(
    TARGETS ${CMAKE_PROJECT_NAME}CLI
    DESTINATION
    ${CMAKE_INSTALL_BINDIR}

    LIBRARY
    DESTINATION
    ${CMAKE_INSTALL_LIBDIR}

    RUNTIME
    DESTINATION
    ${CMAKE_INSTALL_BINDIR}
    COMPONENT
    runtime
)
