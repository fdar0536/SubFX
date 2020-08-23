set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 5.14 COMPONENTS Widgets REQUIRED)

add_executable(${CMAKE_PROJECT_NAME}GUI WIN32
    main.cpp
    ../common/configparser.hpp
    ../common/configparser.cpp
    ../common/print_script_template.hpp
    ui/mainwindow.hpp
    ui/mainwindow.cpp
    ui/mainwindow.ui
)

set(${CMAKE_PROJECT_NAME}Utils_DIR "${CMAKE_BINARY_DIR}/utils")
find_package(${CMAKE_PROJECT_NAME}Utils CONFIG REQUIRED)
add_dependencies(${CMAKE_PROJECT_NAME}GUI ${CMAKE_PROJECT_NAME}Utils)

set(${CMAKE_PROJECT_NAME}YutilsCpp_DIR "${CMAKE_BINARY_DIR}/YutilsCpp")
find_package(${CMAKE_PROJECT_NAME}YutilsCpp CONFIG REQUIRED)
add_dependencies(${CMAKE_PROJECT_NAME}GUI ${CMAKE_PROJECT_NAME}YutilsCpp)

add_dependencies(${CMAKE_PROJECT_NAME}GUI BaseCommon)

LIST(APPEND ${CMAKE_PROJECT_NAME}GUI_includes ${nlohmann_json_INCLUDE_DIRS})
LIST(APPEND ${CMAKE_PROJECT_NAME}GUI_includes ${PYTHON_INCLUDE_DIRS})
LIST(APPEND ${CMAKE_PROJECT_NAME}GUI_includes ${pybind11_INCLUDE_DIRS})
LIST(APPEND ${CMAKE_PROJECT_NAME}GUI_libs ${PYTHON_LIBRARIES})
LIST(APPEND ${CMAKE_PROJECT_NAME}GUI_libs ${CMAKE_PROJECT_NAME}::Utils)

set(Qt5_libs Qt5::Widgets)

target_link_libraries(${CMAKE_PROJECT_NAME}GUI PRIVATE ${Qt5_libs} ${SubFXGUI_libs})
target_include_directories(${CMAKE_PROJECT_NAME}GUI
    SYSTEM BEFORE
    PUBLIC
    ${${CMAKE_PROJECT_NAME}GUI_includes}
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)
