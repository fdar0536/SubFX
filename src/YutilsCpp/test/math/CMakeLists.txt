add_executable(testYMath
    main.cpp
    testingcase.cpp
)

add_dependencies(testYMath YutilsCpp)
target_link_libraries(testYMath YutilsCpp)
