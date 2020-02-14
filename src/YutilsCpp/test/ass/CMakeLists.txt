add_executable(testAss
    main.cpp
)

add_dependencies(testAss YutilsCpp)
target_link_libraries(testAss YutilsCpp)
