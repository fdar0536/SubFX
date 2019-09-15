find_package(pybind11 2.3.0 REQUIRED)
add_library(CoreLibPy SHARED
    CoreLib/python/assparserpy.cpp
    CoreLib/python/corelibpy.cpp
)

add_dependencies(CoreLibPy CoreLib)

#python
LIST(APPEND CoreLibPy_includes ${PYTHON_INCLUDE_DIRS})
LIST(APPEND CoreLibPy_includes ${pybind11_INCLUDE_DIRS})
LIST(APPEND CoreLibPy_libs ${PYTHON_LIBRARIES})

target_link_libraries(CoreLibPy ${CoreLibPy_libs} CoreLib)
target_include_directories(CoreLibPy PUBLIC ${CoreLibPy_includes})

set_target_properties(CoreLibPy PROPERTIES PREFIX "")

if (WIN32)
    set_target_properties(CoreLibPy PROPERTIES SUFFIX ".pyd")
endif (WIN32)
