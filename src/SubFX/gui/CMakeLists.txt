set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(SubFXGUI WIN32
    main.cpp
    ui/mainwindow.hpp
    ui/mainwindow.cpp
    ui/mainwindow.ui
    ui/mainpanel.cpp
    ui/mainpanel.hpp
    ui/mainpanel.ui
    ui/logpanel.cpp
    ui/logpanel.hpp
    ui/logpanel.ui
)

add_dependencies(SubFXGUI YutilsCpp)

LIST(APPEND SubFXGUI_includes ${nlohmann_json_INCLUDE_DIRS})
LIST(APPEND SubFXGUI_includes ${PYTHON_INCLUDE_DIRS})
LIST(APPEND SubFXGUI_includes ${pybind11_INCLUDE_DIRS})

LIST(APPEND SubFXGUI_libs ${PYTHON_LIBRARIES})
LIST(APPEND SubFXGUI_libs YutilsCpp)

set(Qt5_libs Qt5::Widgets)

target_link_libraries(SubFXGUI PRIVATE ${Qt5_libs} ${SubFXGUI_libs})
target_include_directories(SubFXGUI PUBLIC ${SubFXGUI_includes})
