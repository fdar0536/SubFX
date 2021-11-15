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
    include/internal/misc.h
    include/internal/smath.h
    include/internal/utf8.h

    include/internal/ass.h
    include/internal/ass/data.h
    include/internal/assparser.h
    include/internal/fonthandle.h
)

set(subfx_priv_headers
    SubFX/common.h
    SubFX/global.h
    SubFX/mutex.h
    SubFX/regex.h
    SubFX/subfx.h
    SubFX/logger.h
    SubFX/misc.h
    SubFX/smath.h
    SubFX/utf8.h

    SubFX/ass.h
    SubFX/assregex.h
    SubFX/ass/data.h
    SubFX/assparser.h
    SubFX/assparserregex.h
    SubFX/fonthandle.h

    ${CMAKE_BINARY_DIR}/config.h
)

set(subfx_src
    SubFX/global.c
    SubFX/init.c
    SubFX/subfx.c
    SubFX/logger.c
    SubFX/init.c
    SubFX/misc.c
    SubFX/mutex.c
    SubFX/regex.c
    SubFX/smath.c
    SubFX/utf8.c

    SubFX/ass.c
    SubFX/ass/data.c
    SubFX/assparser.c
    SubFX/fonthandle.c
)

add_library(SubFX
    SHARED

    ${subfx_main_header}
    ${subfx_headers}
    ${subfx_priv_headers}
    ${subfx_src}
)

#PCRE2
LIST(APPEND SubFX_includes ${PCRE2_INCLUDE_DIRS})
LIST(APPEND SubFX_libs ${PCRE2_LIBRARIES})

#fDSA
LIST(APPEND SubFX_includes ${fDSA_INCLUDE_DIR})
LIST(APPEND SubFX_libs fDSA::fDSA)

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
