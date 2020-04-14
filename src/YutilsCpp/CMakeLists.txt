if (UNIX)
    include(FindPkgConfig)
    pkg_check_modules(PANGOCAIRO REQUIRED pangocairo>=1.44.6)
    LIST(APPEND YutilsCpp_includes ${PANGOCAIRO_INCLUDE_DIRS})
    LIST(APPEND YutilsCpp_libs ${PANGOCAIRO_LIBRARIES})
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

target_link_libraries(YutilsCpp ${YutilsCpp_libs})
target_include_directories(YutilsCpp PUBLIC ${YutilsCpp_includes})

# python
option(ENABLE_PYTHON "Enable python binding" OFF)

option(BUILD_TESTING_CASES "Build testing cases" ON)

if (ENABLE_PYTHON)
    include(YutilsCpp/python/CMakeLists.txt)
endif (ENABLE_PYTHON)

if (BUILD_TESTING_CASES)
    include(YutilsCpp/test/CMakeLists.txt)
endif (BUILD_TESTING_CASES)

