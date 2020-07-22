find_package(pybind11 2.3.0 REQUIRED)

add_library(${CMAKE_PROJECT_NAME}_YutilsPy SHARED
    yutilspy.cpp
)

add_dependencies(${CMAKE_PROJECT_NAME}_YutilsPy ${CMAKE_PROJECT_NAME}YutilsCpp)

#python
LIST(APPEND YutilsPy_includes ${PYTHON_INCLUDE_DIRS})
LIST(APPEND YutilsPy_includes ${pybind11_INCLUDE_DIRS})
LIST(APPEND YutilsPy_libs ${PYTHON_LIBRARIES})

target_link_libraries(${CMAKE_PROJECT_NAME}_YutilsPy
    ${YutilsPy_libs}
    ${CMAKE_PROJECT_NAME}YutilsCpp
)
target_include_directories(${CMAKE_PROJECT_NAME}_YutilsPy
    SYSTEM BEFORE
    PUBLIC
    ${YutilsPy_includes}
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${Boost_INCLUDE_DIRS}>
)

set_target_properties(${CMAKE_PROJECT_NAME}_YutilsPy PROPERTIES PREFIX "")

if (WIN32)
    set_target_properties(${CMAKE_PROJECT_NAME}_YutilsPy PROPERTIES SUFFIX ".pyd")
endif (WIN32)
