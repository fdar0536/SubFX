add_executable(testLogger
    main.c
)

add_dependencies(testLogger SubFX)
target_link_libraries(testLogger PRIVATE SubFX)
target_include_directories(testLogger
    SYSTEM BEFORE
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
)

add_test(SubFXLogger testLogger)
