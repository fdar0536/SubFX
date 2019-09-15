add_executable(SubFXCLI
    main.cpp
    ../common/configparser.cpp
)

add_dependencies(SubFXCLI CoreLibPy)

LIST(APPEND SubFXCLI_includes ${nlohmann_json_INCLUDE_DIRS})

target_link_libraries(SubFXCLI CoreLibPy)
target_include_directories(SubFXCLI PUBLIC ${SubFXCLI_includes})
