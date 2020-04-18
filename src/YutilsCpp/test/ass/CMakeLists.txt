add_executable(testAss
    main.cpp
)

add_dependencies(testAss YutilsCpp)
target_link_libraries(testAss YutilsCpp)
target_include_directories(testAss
    SYSTEM BEFORE
    PRIVATE
    ${CMAKE_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

if (WIN32)
    if (MINGW)
        add_custom_command(
            TARGET testAss
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_BINARY_DIR}/libSubFXUtils.dll"
            "${CMAKE_BINARY_DIR}/libSubFXUtils.dll.a"
            "${CMAKE_BINARY_DIR}/libYutilsCpp.dll"
            "${CMAKE_BINARY_DIR}/libYutilsCpp.dll.a"
            $<TARGET_FILE_DIR:testAss>)
    endif(MINGW)
endif(WIN32)

add_test(YutilsCppAss testAss)
