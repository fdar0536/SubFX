add_executable(testYMath
    main.cpp
    testingcase.hpp
    testingcase.cpp
)

add_dependencies(testYMath YutilsCpp)
target_link_libraries(testYMath YutilsCpp)
target_include_directories(testYMath
    SYSTEM BEFORE
    PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

add_test(YutilsCppMath testYMath)
