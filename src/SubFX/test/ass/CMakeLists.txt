add_executable(testAss
    main.c
)

add_dependencies(testAss SubFX)
target_link_libraries(testAss PRIVATE SubFX)
target_include_directories(testAss
    SYSTEM BEFORE
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
)

if (WIN32)
    if (MINGW)
        add_custom_command(
            TARGET testAss
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_BINARY_DIR}/utils/libSubFXUtils.dll"
            "${CMAKE_BINARY_DIR}/utils/libSubFXUtils.dll.a"
            "${CMAKE_BINARY_DIR}/SubFX/libSubFXSubFX.dll"
            "${CMAKE_BINARY_DIR}/SubFX/libSubFXSubFX.dll.a"
            $<TARGET_FILE_DIR:testAss>)
    endif(MINGW)
endif(WIN32)

add_test(SubFXAss testAss)
