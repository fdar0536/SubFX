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
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

configure_file(in.ass.in in.ass @ONLY)

# add_test(YutilsCppAss testAssParser)
