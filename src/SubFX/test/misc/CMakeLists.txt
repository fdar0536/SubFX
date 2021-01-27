add_executable(testMisc
    main.c
)

add_dependencies(testMisc SubFX)
target_link_libraries(testMisc PRIVATE SubFX)
target_include_directories(testMisc
    SYSTEM BEFORE
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
)

configure_file(in.ass.in in.ass @ONLY)

if (WIN32)
    if (MINGW)
        add_custom_command(
            TARGET testAss
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_BINARY_DIR}//libSubFX.dll"
            "${CMAKE_BINARY_DIR}//libSubFX.dll.a"
            $<TARGET_FILE_DIR:testVector>)
    endif(MINGW)
endif(WIN32)

add_test(SubFXMisc testMisc)
