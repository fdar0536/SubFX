if (UNIX)
    include(FindPkgConfig)
    pkg_check_modules(PANGOCAIRO REQUIRED pangocairo)
    LIST(APPEND CoreLib_includes ${PANGOCAIRO_INCLUDE_DIRS})
    LIST(APPEND CoreLib_libs ${PANGOCAIRO_LIBRARIES})
endif (UNIX)

option(enable_python "Enable python binding" ON)

if (enable_python)
    find_package(pybind11 REQUIRED)
    add_library(CoreLib SHARED
        ass.cpp
        assparser.cpp
        common.cpp
        file.cpp
        fonthandle.cpp
        math.cpp
        shape.cpp
        utf8.cpp
        python/assparserpy.cpp
        python/corelib.cpp
    )
    
    #python
    LIST(APPEND CoreLib_includes ${PYTHON_INCLUDE_DIRS})
    LIST(APPEND CoreLib_includes ${pybind11_INCLUDE_DIRS})
    LIST(APPEND CoreLib_libs ${PYTHON_LIBRARIES})
    
else ()
    add_library(CoreLib SHARED
        ass.cpp
        assparser.cpp
        common.cpp
        file.cpp
        fonthandle.cpp
        math.cpp
        shape.cpp
        utf8.cpp
    )
endif (enable_python)

if (ENABLE_SSE2)
    if (UNIX)
        set_source_files_properties(shape.cpp PROPERTIES
            COMPILE_FLAGS ${CMAKE_CXX_FLAGS} -msse2)
    else () # windows
        if (MINGW)
            set_source_files_properties(shape.cpp PROPERTIES
                                        COMPILE_FLAGS ${CMAKE_CXX_FLAGS} -msse2)
        endif (MINGW)
    endif (UNIX)
endif (ENABLE_SSE2)

#ICU
LIST(APPEND CoreLib_includes ${ICU_INCLUDE_DIRS})
LIST(APPEND CoreLib_libs ${ICU_LIBRARIES})

# boost
LIST(APPEND CoreLib_includes ${Boost_INCLUDE_DIRS})
LIST(APPEND CoreLib_libs ${Boost_REGEX_LIBRARY})
if (WIN32)
    LIST(APPEND CoreLib_libs ${Boost_LOCALE_LIBRARY})
endif (WIN32)

target_link_libraries(CoreLib ${CoreLib_libs})
target_include_directories(CoreLib PUBLIC ${CoreLib_includes})
set_target_properties(CoreLib PROPERTIES PREFIX "")

if (WIN32)
    set_target_properties(CoreLib PROPERTIES SUFFIX ".pyd")
endif (WIN32)
