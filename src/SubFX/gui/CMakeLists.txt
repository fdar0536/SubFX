set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable(SubFXGUI WIN32
    main.cpp
)

add_dependencies(SubFXGUI YutilsCpp)

LIST(APPEND SubFXGUI_includes ${nlohmann_json_INCLUDE_DIRS})
LIST(APPEND SubFXGUI_includes ${PYTHON_INCLUDE_DIRS})
LIST(APPEND SubFXGUI_includes ${pybind11_INCLUDE_DIRS})

LIST(APPEND SubFXGUI_libs ${PYTHON_LIBRARIES})
LIST(APPEND SubFXGUI_libs YutilsCpp)
LIST(APPEND SubFXGUI_libs Qt5::Widgets)

target_link_libraries(SubFXGUI PRIVATE ${SubFXCLI_libs})
target_include_directories(SubFXGUI PUBLIC ${SubFXCLI_includes})
