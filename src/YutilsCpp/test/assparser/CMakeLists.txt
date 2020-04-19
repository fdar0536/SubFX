add_executable(testAssParser
    main.cpp
    assdumper.hpp
    assdumper.cpp
)

add_dependencies(testAssParser ${CMAKE_PROJECT_NAME}YutilsCpp)
target_link_libraries(testAssParser PRIVATE ${CMAKE_PROJECT_NAME}YutilsCpp)
target_include_directories(testAssParser
    SYSTEM BEFORE
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
)

configure_file(in.ass.in in.ass @ONLY)

if (WIN32)
    if (MINGW)
        add_custom_command(
            TARGET testAssParser
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_BINARY_DIR}/utils/libSubFXUtils.dll"
            "${CMAKE_BINARY_DIR}/utils/libSubFXUtils.dll.a"
            "${CMAKE_BINARY_DIR}/YutilsCpp/libSubFXYutilsCpp.dll"
            "${CMAKE_BINARY_DIR}/YutilsCpp/libSubFXYutilsCpp.dll.a"
            $<TARGET_FILE_DIR:testAssParser>)
    endif(MINGW)
endif(WIN32)

add_test(YutilsCppAssParser testAssParser)
