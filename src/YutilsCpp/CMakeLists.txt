if (UNIX)
    find_package(PANGO REQUIRED)
    if (NOT PANGOCAIRO_FOUND)
        message(FATAL_ERROR "CANNOT find PangoCairo!")
    endif()
    LIST(APPEND YutilsCpp_includes
        ${PANGOCAIRO_INCLUDE_DIRS}
        ${PANGO_INCLUDE_DIRS}
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

set(YutilsCpp_public_header
    # ${YUTILSCPP_INCLUDE_PREFIX}/ass.hpp
    # ass.cpp
    # ${YUTILSCPP_INCLUDE_PREFIX}/asscommon.hpp
    # ${YUTILSCPP_INCLUDE_PREFIX}/assparser.hpp
    # assparser.cpp
    # ${YUTILSCPP_INCLUDE_PREFIX}/asswriter.hpp
    # asswriter.cpp
    # internal/asswriter_internal.hpp
    # internal/asswriter_internal.cpp
    # ${YUTILSCPP_INCLUDE_PREFIX}/fonthandle.hpp
    # fonthandle.cpp
    ${CMAKE_SOURCE_DIR}/include/internal/yutilscpp/math.h
    # ${YUTILSCPP_INCLUDE_PREFIX}/matrix.hpp
    # matrix.cpp
    # ${YUTILSCPP_INCLUDE_PREFIX}/shape.hpp
    # shape.cpp
    # internal/shape_internal.hpp
    # internal/shape_internal.cpp
    ${CMAKE_SOURCE_DIR}/include/internal/yutils.h
)

set(YutilsCpp_priv_header
    YutilsCpp/math.h
)

set(YutilsCpp_src
    YutilsCpp/math.c
)

#ICU
LIST(APPEND YutilsCpp_includes ${ICU_INCLUDE_DIRS})
LIST(APPEND YutilsCpp_libs ${ICU_LIBRARIES})

# boost
LIST(APPEND YutilsCpp_libs ${Boost_REGEX_LIBRARY})
if (WIN32)
    LIST(APPEND YutilsCpp_libs ${Boost_LOCALE_LIBRARY})
endif (WIN32)

# eigen
find_package(Eigen3 3.3.7 REQUIRED)
LIST(APPEND YutilsCpp_libs Eigen3::Eigen)

# BLAS
if (ENABLE_BLAS)
    find_package(BLAS REQUIRED)
    LIST(APPEND YutilsCpp_libs BLAS::BLAS)
endif (ENABLE_BLAS)

# if (ENABLE_PYTHON)
#     add_subdirectory(python)
# endif (ENABLE_PYTHON)

# if (BUILD_TESTING_CASES)
#     include(test/CMakeLists.txt)
# endif (BUILD_TESTING_CASES)
