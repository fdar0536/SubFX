add_executable(SubFXCLI
    main.cpp
    launcher.cpp
    asslauncher.cpp
    ../common/configparser.cpp
    ../common/subfxassinit.cpp
    ../common/assparserpy.cpp
)

add_dependencies(SubFXCLI YutilsCpp)

LIST(APPEND SubFXCLI_includes ${nlohmann_json_INCLUDE_DIRS})
LIST(APPEND SubFXCLI_includes ${PYTHON_INCLUDE_DIRS})
LIST(APPEND SubFXCLI_includes ${pybind11_INCLUDE_DIRS})
LIST(APPEND SubFXCLI_libs ${PYTHON_LIBRARIES})

target_link_libraries(SubFXCLI PRIVATE pybind11::embed ${SubFXCLI_libs} YutilsCpp)
target_include_directories(SubFXCLI PUBLIC ${SubFXCLI_includes})
