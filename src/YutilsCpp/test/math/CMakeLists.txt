add_executable(testYMath
    main.cpp
    testingcase.hpp
    testingcase.cpp
)

add_dependencies(testYMath ${CMAKE_PROJECT_NAME}YutilsCpp)
target_link_libraries(testYMath PRIVATE ${CMAKE_PROJECT_NAME}YutilsCpp)
target_include_directories(testYMath
    SYSTEM BEFORE
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
)

if (WIN32)
    if (MINGW)
        add_custom_command(
            TARGET testYMath
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_BINARY_DIR}/utils/libSubFXUtils.dll"
            "${CMAKE_BINARY_DIR}/utils/libSubFXUtils.dll.a"
            "${CMAKE_BINARY_DIR}/YutilsCpp/libYutilsCpp.dll"
            "${CMAKE_BINARY_DIR}/YutilsCpp/libYutilsCpp.dll.a"
            $<TARGET_FILE_DIR:testYMath>)
    endif(MINGW)
endif(WIN32)

add_test(YutilsCppMath testYMath)
