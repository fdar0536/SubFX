if (UNIX)
    include(FindPkgConfig)
    pkg_check_modules(PANGOCAIRO REQUIRED pangocairo>=1.44.6)
    LIST(APPEND YutilsCpp_includes ${PANGOCAIRO_INCLUDE_DIRS})
    LIST(APPEND YutilsCpp_libs ${PANGOCAIRO_LIBRARIES})
endif (UNIX)

add_library(YutilsCpp SHARED
    YutilsCpp/ass.cpp
    YutilsCpp/assparser.cpp
    YutilsCpp/common.cpp
    YutilsCpp/file.cpp
    YutilsCpp/fonthandle.cpp
    YutilsCpp/math.cpp
    YutilsCpp/shape.cpp
    YutilsCpp/utf8.cpp
)

if (ENABLE_SSE)
    if (UNIX)
        set_source_files_properties(shape.cpp PROPERTIES
            COMPILE_FLAGS ${CMAKE_CXX_FLAGS} -msse)
    else () # windows
        if (MINGW)
            set_source_files_properties(shape.cpp PROPERTIES
                                        COMPILE_FLAGS ${CMAKE_CXX_FLAGS} -msse)
        endif (MINGW)
    endif (UNIX)
endif (ENABLE_SSE2)

#ICU
LIST(APPEND YutilsCpp_includes ${ICU_INCLUDE_DIRS})
LIST(APPEND YutilsCpp_libs ${ICU_LIBRARIES})

# boost
LIST(APPEND YutilsCpp_includes ${Boost_INCLUDE_DIRS})
LIST(APPEND YutilsCpp_libs ${Boost_REGEX_LIBRARY})
if (WIN32)
    LIST(APPEND YutilsCpp_libs ${Boost_LOCALE_LIBRARY})
endif (WIN32)

target_link_libraries(YutilsCpp ${YutilsCpp_libs})
target_include_directories(YutilsCpp PUBLIC ${YutilsCpp_includes})

# python
option(ENABLE_PYTHON "Enable python binding" ON)

if (ENABLE_PYTHON)
    include(YutilsCpp/python/CMakeLists.txt)
endif (ENABLE_PYTHON)
