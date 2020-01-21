find_package(pybind11 2.3.0 REQUIRED)

add_library(YutilsPy SHARED
    YutilsCpp/python/yutilspy.cpp
)

add_dependencies(YutilsPy YutilsCpp)

#python
LIST(APPEND YutilsPy_includes ${PYTHON_INCLUDE_DIRS})
LIST(APPEND YutilsPy_includes ${pybind11_INCLUDE_DIRS})
LIST(APPEND YutilsPy_libs ${PYTHON_LIBRARIES})

target_link_libraries(YutilsPy ${YutilsPy_libs} YutilsCpp)
target_include_directories(YutilsPy PUBLIC ${YutilsPy_includes})

set_target_properties(YutilsPy PROPERTIES PREFIX "")

if (WIN32)
    set_target_properties(YutilsPy PROPERTIES SUFFIX ".pyd")
endif (WIN32)
