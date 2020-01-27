# gui and cli
option(ENABLE_CLI "Enable CLI frontend" ON)
option(ENABLE_GUI "Enable GUI frontend" ON)
if (ENABLE_CLI OR ENABLE_GUI)
    find_package(nlohmann_json 3.7.0 REQUIRED)
endif (ENABLE_CLI OR ENABLE_GUI)

if (ENABLE_CLI)
    add_subdirectory(SubFX/cli)
endif (ENABLE_CLI)

if (ENABLE_GUI)
    add_subdirectory(SubFX/gui)
endif (ENABLE_GUI)
