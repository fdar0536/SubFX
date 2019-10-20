add_executable(SubFXCLI
    main.cpp
    launcher.cpp
    asslauncher.cpp
    ../common/configparser.cpp
    ../common/subfxassinit.cpp
)

add_dependencies(SubFXCLI AssParserPy)

LIST(APPEND SubFXCLI_includes ${nlohmann_json_INCLUDE_DIRS})

target_link_libraries(SubFXCLI AssParserPy)
target_include_directories(SubFXCLI PUBLIC ${SubFXCLI_includes})
