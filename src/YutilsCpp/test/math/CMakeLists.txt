add_executable(testYutilsCppMath
    main.c
    testingcase.h
    testingcase.c
)

add_dependencies(testYutilsCppMath SubFX)
target_link_libraries(testYutilsCppMath PRIVATE SubFX)
target_include_directories(testYutilsCppMath
    SYSTEM BEFORE
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
)

if (WIN32)
    if (MINGW)
        add_custom_command(
            TARGET testYutilsCppMath
            POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_BINARY_DIR}/utils/libSubFXUtils.dll"
            "${CMAKE_BINARY_DIR}/utils/libSubFXUtils.dll.a"
            "${CMAKE_BINARY_DIR}/YutilsCpp/libSubFXYutilsCpp.dll"
            "${CMAKE_BINARY_DIR}/YutilsCpp/libSubFXYutilsCpp.dll.a"
            $<TARGET_FILE_DIR:testYutilsCppMath>)
    endif(MINGW)
endif(WIN32)

add_test(YutilsCppMath testYutilsCppMath)
