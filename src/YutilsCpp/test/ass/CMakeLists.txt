add_executable(testAss
    main.cpp
)

add_dependencies(testAss YutilsCpp)
target_link_libraries(testAss YutilsCpp)
target_include_directories(testAss
    SYSTEM BEFORE
    PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

add_test(YutilsCppAss testAss)
