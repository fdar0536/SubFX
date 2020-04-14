if (UNIX)
    find_package(PANGO REQUIRED)
    if (NOT PANGOCAIRO_FOUND)
        message(FATAL_ERROR "CANNOT find PangoCairo!")
    endif()
    LIST(APPEND YutilsCpp_includes
        ${PANGOCAIRO_INCLUDE_DIRS}
        ${GLIB2_INCLUDE_DIRS})
    LIST(APPEND YutilsCpp_libs
        ${PANGO_LIBRARIES}
        ${PANGOCAIRO_LIBRARIES}
        ${GLIB2_LIBRARIES})

    find_package(HarfBuzz REQUIRED)
    LIST(APPEND YutilsCpp_includes
        ${HARFBUZZ_INCLUDE_DIRS})
    LIST(APPEND YutilsCpp_libs
        ${HARFBUZZ_LIBRARIES})

    find_package(Cairo REQUIRED)
    LIST(APPEND YutilsCpp_includes
        ${CAIRO_INCLUDE_DIRS})
    LIST(APPEND YutilsCpp_libs
        ${CAIRO_LIBRARIES})
endif (UNIX)

set(YUTILSCPP_INCLUDE_PREFIX include/internal/yutilscpp)

add_library(YutilsCpp SHARED
    ${YUTILSCPP_INCLUDE_PREFIX}/ass.hpp
    YutilsCpp/ass.cpp
    ${YUTILSCPP_INCLUDE_PREFIX}/asscommon.hpp
    # YutilsCpp/assparser.cpp
    ${YUTILSCPP_INCLUDE_PREFIX}/common.hpp
    YutilsCpp/common.cpp
    # YutilsCpp/file.cpp
    ${YUTILSCPP_INCLUDE_PREFIX}/fonthandle.hpp
    YutilsCpp/fonthandle.cpp
    ${YUTILSCPP_INCLUDE_PREFIX}/math.hpp
    YutilsCpp/math.cpp
    # YutilsCpp/shape.cpp
    ${YUTILSCPP_INCLUDE_PREFIX}/utf8.hpp
    YutilsCpp/utf8.cpp
    include/YutilsCpp
    include/internal/basecommon.h
)

#ICU
LIST(APPEND YutilsCpp_includes ${ICU_INCLUDE_DIRS})
LIST(APPEND YutilsCpp_libs ${ICU_LIBRARIES})

# boost
LIST(APPEND YutilsCpp_includes ${Boost_INCLUDE_DIRS})
LIST(APPEND YutilsCpp_libs ${Boost_REGEX_LIBRARY})
if (WIN32)
    LIST(APPEND YutilsCpp_libs ${Boost_LOCALE_LIBRARY})
endif (WIN32)

#config and others
LIST(APPEND YutilsCpp_includes
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(YutilsCpp ${YutilsCpp_libs})
target_include_directories(YutilsCpp
    SYSTEM BEFORE
    PUBLIC ${YutilsCpp_includes}
)

# python
option(ENABLE_PYTHON "Enable python binding" OFF)

option(BUILD_TESTING_CASES "Build testing cases" ON)

if (ENABLE_PYTHON)
    include(YutilsCpp/python/CMakeLists.txt)
endif (ENABLE_PYTHON)

if (BUILD_TESTING_CASES)
    include(YutilsCpp/test/CMakeLists.txt)
endif (BUILD_TESTING_CASES)

