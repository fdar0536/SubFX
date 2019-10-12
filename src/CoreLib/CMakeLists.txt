if (UNIX)
    include(FindPkgConfig)
    pkg_check_modules(PANGOCAIRO REQUIRED pangocairo>=1.44.6)
    LIST(APPEND CoreLib_includes ${PANGOCAIRO_INCLUDE_DIRS})
    LIST(APPEND CoreLib_libs ${PANGOCAIRO_LIBRARIES})
endif (UNIX)

add_library(CoreLib SHARED
    CoreLib/ass.cpp
    CoreLib/assparser.cpp
    CoreLib/common.cpp
    CoreLib/file.cpp
    CoreLib/fonthandle.cpp
    CoreLib/math.cpp
    CoreLib/shape.cpp
    CoreLib/utf8.cpp
)

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

# python
option(ENABLE_PYTHON "Enable python binding" ON)

if (ENABLE_PYTHON)
    include(CoreLib/python/CMakeLists.txt)
endif (ENABLE_PYTHON)
