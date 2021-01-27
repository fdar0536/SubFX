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
)

add_library(SubFX
    SHARED

    ${subfx_main_header}
    ${subfx_headers}
    ${subfx_priv_headers}
    ${subfx_src}
)

target_include_directories(SubFX
    SYSTEM BEFORE
    PUBLIC
    ${YutilsCpp_includes}
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
    $<BUILD_INTERFACE:${Boost_INCLUDE_DIRS}>
)

target_link_libraries(SubFX PUBLIC ${YutilsCpp_libs})
