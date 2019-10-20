find_package(pybind11 2.3.0 REQUIRED)

add_library(AssParserPy SHARED
    YutilsCpp/python/assparserpy.cpp
)

add_library(YutilsPy SHARED
    YutilsCpp/python/yutilspy.cpp
)

add_dependencies(AssParserPy YutilsCpp)
add_dependencies(YutilsPy AssParserPy)

#python
LIST(APPEND AssParserPy_includes ${PYTHON_INCLUDE_DIRS})
LIST(APPEND AssParserPy_includes ${pybind11_INCLUDE_DIRS})
LIST(APPEND AssParserPy_libs ${PYTHON_LIBRARIES})

target_link_libraries(AssParserPy ${AssParserPy_libs} YutilsCpp)
target_include_directories(AssParserPy PUBLIC ${AssParserPy_includes})

LIST(APPEND YutilsPy_includes ${PYTHON_INCLUDE_DIRS})
LIST(APPEND YutilsPy_includes ${pybind11_INCLUDE_DIRS})
LIST(APPEND YutilsPy_libs ${PYTHON_LIBRARIES})

target_link_libraries(YutilsPy ${YutilsPy_libs} AssParserPy)
target_include_directories(YutilsPy PUBLIC ${YutilsPy_includes})

set_target_properties(YutilsPy PROPERTIES PREFIX "")

if (WIN32)
    set_target_properties(YutilsPy PROPERTIES SUFFIX ".pyd")
endif (WIN32)
