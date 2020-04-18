add_executable(testAssParser
    main.cpp
    assdumper.hpp
    assdumper.cpp
)

add_dependencies(testAssParser YutilsCpp)
target_link_libraries(testAssParser YutilsCpp)
target_include_directories(testAssParser
    SYSTEM BEFORE
    PRIVATE
    ${CMAKE_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

configure_file(in.ass.in in.ass @ONLY)

if (WIN32)
    if (MINGW)
        add_custom_command(
            TARGET testAssParser
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_BINARY_DIR}/libSubFXUtils.dll"
            "${CMAKE_BINARY_DIR}/libSubFXUtils.dll.a"
            "${CMAKE_BINARY_DIR}/libYutilsCpp.dll"
            "${CMAKE_BINARY_DIR}/libYutilsCpp.dll.a"
            $<TARGET_FILE_DIR:testAssParser>)
    endif(MINGW)
endif(WIN32)

add_test(YutilsCppAssParser testAssParser)
