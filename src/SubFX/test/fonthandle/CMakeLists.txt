add_executable(testFontHandle
    main.c
)

add_dependencies(testFontHandle SubFX)
target_link_libraries(testFontHandle PRIVATE SubFX)
target_include_directories(testFontHandle
    SYSTEM BEFORE
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
)

if (WIN32)
    if (MINGW)
        add_custom_command(
            TARGET testFontHandle
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_BINARY_DIR}/utils/libUtils.dll"
            "${CMAKE_BINARY_DIR}/utils/libUtils.dll.a"
            "${CMAKE_BINARY_DIR}//lib.dll"
            "${CMAKE_BINARY_DIR}//lib.dll.a"
            $<TARGET_FILE_DIR:testFontHandle>)
    endif (MINGW)
endif(WIN32)

add_test(SubFXFontHandle testFontHandle)
