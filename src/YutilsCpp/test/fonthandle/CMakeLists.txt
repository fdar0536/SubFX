add_executable(testFontHandle
    main.cpp
)

add_dependencies(testFontHandle YutilsCpp)
target_link_libraries(testFontHandle YutilsCpp)
