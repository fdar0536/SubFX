add_executable(testUtilsLogger
    main.c
)

add_dependencies(testUtilsLogger SubFX)
target_link_libraries(testUtilsLogger PRIVATE SubFX)
target_include_directories(testUtilsLogger
    SYSTEM BEFORE
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
)

add_test(SubFXUtilsLogger testUtilsLogger)
