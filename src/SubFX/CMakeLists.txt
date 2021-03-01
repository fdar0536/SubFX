if (UNIX)
    find_package(PANGO REQUIRED)
    if (NOT PANGOCAIRO_FOUND)
        message(FATAL_ERROR "CANNOT find PangoCairo!")
    endif()
    LIST(APPEND SubFX_includes
        ${PANGOCAIRO_INCLUDE_DIRS}
        ${PANGO_INCLUDE_DIRS}
        ${GLIB2_INCLUDE_DIRS})
    LIST(APPEND SubFX_libs
        ${PANGO_LIBRARIES}
        ${PANGOCAIRO_LIBRARIES}
        ${GLIB2_LIBRARIES})

    find_package(HarfBuzz REQUIRED)
    LIST(APPEND SubFX_includes
        ${HARFBUZZ_INCLUDE_DIRS})
    LIST(APPEND SubFX_libs
        ${HARFBUZZ_LIBRARIES})

    find_package(Cairo REQUIRED)
    LIST(APPEND SubFX_includes
        ${CAIRO_INCLUDE_DIRS})
    LIST(APPEND SubFX_libs
        ${CAIRO_LIBRARIES})
endif (UNIX)

set(subfx_main_header
    include/SubFX.h
)

set(subfx_headers
    include/internal/defines.h
    include/internal/logger.h
    include/internal/map.h
    include/internal/misc.h
    include/internal/ptrvector.h
    include/internal/smath.h
    include/internal/utf8.h
    include/internal/vector.h

    include/internal/ass.h
    # include/internal/ass/data.h
    include/internal/fonthandle.h
)

set(subfx_priv_headers
    SubFX/common.h
    SubFX/subfx.h
    SubFX/types.h
    SubFX/logger.h
    SubFX/map.h
    SubFX/misc.h
    SubFX/ptrvector.h
    SubFX/smath.h
    SubFX/utf8.h
    SubFX/vector.h

    SubFX/ass.h
    # SubFX/ass/data.h
    SubFX/fonthandle.h

    ${CMAKE_BINARY_DIR}/config.h
)

set(subfx_src
    SubFX/init.c
    SubFX/subfx.c
    SubFX/types.c
    SubFX/logger.c
    SubFX/init.c
    SubFX/map.c
    SubFX/misc.c
    SubFX/ptrvector.c
    SubFX/smath.c
    SubFX/utf8.c
    SubFX/vector.c

    SubFX/ass.cpp
    # SubFX/ass/data.c
    SubFX/fonthandle.cpp
)

add_library(SubFX
    SHARED

    ${subfx_main_header}
    ${subfx_headers}
    ${subfx_priv_headers}
    ${subfx_src}
)

#ICU
LIST(APPEND SubFX_includes ${ICU_INCLUDE_DIRS})
LIST(APPEND SubFX_libs ${ICU_LIBRARIES})

# boost
LIST(APPEND SubFX_libs ${Boost_REGEX_LIBRARY})
if (WIN32)
    LIST(APPEND SubFX_libs ${Boost_LOCALE_LIBRARY})
endif (WIN32)

# eigen
find_package(Eigen3 3.3.7 REQUIRED)
LIST(APPEND SubFX_libs Eigen3::Eigen)

# BLAS
if (ENABLE_BLAS)
    find_package(BLAS REQUIRED)
    LIST(APPEND SubFX_libs BLAS::BLAS)
endif (ENABLE_BLAS)

# if (ENABLE_PYTHON)
#     add_subdirectory(python)
# endif (ENABLE_PYTHON)

target_include_directories(SubFX
    SYSTEM BEFORE
    PUBLIC
    ${SubFX_includes}
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
    $<BUILD_INTERFACE:${Boost_INCLUDE_DIRS}>
)

target_link_libraries(SubFX PUBLIC ${SubFX_libs})
