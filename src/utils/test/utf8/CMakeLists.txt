add_executable(testUtilsUtf8
    main.c
)

add_dependencies(testUtilsUtf8 SubFX)
target_link_libraries(testUtilsUtf8 PRIVATE SubFX)
target_include_directories(testUtilsUtf8
    SYSTEM BEFORE
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
)

configure_file(test.txt.in test.txt @ONLY)

if (WIN32)
    if (MINGW)
        add_custom_command(
            TARGET testAss
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_BINARY_DIR}/utils/libSubFXUtils.dll"
            "${CMAKE_BINARY_DIR}/utils/libSubFXUtils.dll.a"
            $<TARGET_FILE_DIR:testVector>)
    endif(MINGW)
endif(WIN32)

add_test(SubFXUtilsUtf8 testUtilsUtf8)
