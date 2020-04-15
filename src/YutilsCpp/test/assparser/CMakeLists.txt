add_executable(testAssParser
    main.cpp
)

add_dependencies(testAssParser YutilsCpp)
target_link_libraries(testAssParser YutilsCpp)
target_include_directories(testAssParser
    SYSTEM BEFORE
    PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

# add_test(YutilsCppAss testAssParser)
