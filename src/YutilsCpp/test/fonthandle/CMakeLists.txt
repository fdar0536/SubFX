add_executable(testFontHandle
    main.cpp
)

add_dependencies(testFontHandle YutilsCpp)
target_link_libraries(testFontHandle YutilsCpp)
target_include_directories(testFontHandle
    SYSTEM BEFORE
    PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

add_test(YutilsCppFontHandle testFontHandle)
