cmake_minimum_required(VERSION 3.14)

project(SubFX
    LANGUAGES C CXX
    VERSION 0.0.2
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(GNUInstallDirs)

if (UNIX)
    find_package(Boost 1.69.0 REQUIRED COMPONENTS regex)
    find_package(ICU 64.2 REQUIRED COMPONENTS uc i18n)
else () # windows
    find_package(Boost 1.69.0 REQUIRED COMPONENTS regex locale)
    find_package(ICU 64.2 REQUIRED COMPONENTS uc in)
endif (UNIX)

option(SUBFX_ENABLE_SIMD "Enable X86 SIMD, include SSE" ON)
if(SUBFX_ENABLE_SIMD)
    option(SUBFX_ENABLE_AVX "Enable AVX" ON)
endif(SUBFX_ENABLE_SIMD)

if (UNIX)
    add_compile_options(-Wall -Werror -Wextra -fvisibility=hidden)

    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        add_compile_options(-Weffc++)
    endif ()

    if(SUBFX_ENABLE_SIMD)
        add_compile_options(-msse)
        if (SUBFX_ENABLE_AVX)
            add_compile_options(-mavx)
        endif (SUBFX_ENABLE_AVX)
    endif(SUBFX_ENABLE_SIMD)

    add_link_options(-lm)
    if (NOT APPLE)
        add_link_options(-Wl,--unresolved-symbols=report-all)
    endif (NOT APPLE)
else () # Windows
    if (MINGW)
        add_compile_options(-Wall -Werror -Wextra -fvisibility=hidden)

        if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
            add_compile_options(-Weffc++)
        endif ()

        if(SUBFX_ENABLE_SIMD)
            add_compile_options(-msse)
            if (SUBFX_ENABLE_AVX)
                add_compile_options(-mavx)
            endif (SUBFX_ENABLE_AVX)
        endif(SUBFX_ENABLE_SIMD)

        add_link_options(-Wl,--unresolved-symbols=report-all -lm)
    else () # msvc or icc
        add_compile_options(/W4 /WX /wd4819 /wd4996)
        if(SUBFX_ENABLE_SIMD)
            if (SUBFX_ENABLE_AVX)
                add_compile_options(/arch:AVX)
            endif (SUBFX_ENABLE_AVX)
        endif(SUBFX_ENABLE_SIMD)
    endif (MINGW)
endif (UNIX) # end if (UNIX)

enable_testing()

include(utils/CMakeLists.txt)
include(YutilsCpp/CMakeLists.txt)

set(subfx_main_header
    ${CMAKE_SOURCE_DIR}/include/SubFX.h
)

set(subfx_priv_headers
    internal/common.h
    internal/subfx.h
    internal/types.h
    ${CMAKE_BINARY_DIR}/config.h
)

set(subfx_src
    init.c
    internal/subfx.c
    internal/types.c
)

add_library(SubFX
    SHARED

    #utils
    ${utils_main_headers}
    ${utils_headers}
    ${utils_priv_headers}
    ${utils_src}

    #YutilsCpp
    ${YutilsCpp_main_headers}
    ${YutilsCpp_headers}
    ${YutilsCpp_priv_headers}
    ${YutilsCpp_src}

    #api entry
    ${subfx_main_header}
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

option(BUILD_TESTING_CASES "Build testing cases" ON)

# BLAS
#option(SUBFX_ENABLE_BLAS "Enable BLAS backend" ON)

# python
#option(ENABLE_PYTHON "Enable python binding" ON)
#if (ENABLE_PYTHON)
#    find_package(pybind11 2.3.0 REQUIRED)
#    find_package(Python 3.7.0 REQUIRED COMPONENTS Interpreter Development)
#endif(ENABLE_PYTHON)

configure_file(config.h.in config.h @ONLY)

# wrapper
# if (ENABLE_PYTHON)
#     include(SubFX/CMakeLists.txt)
# endif (ENABLE_PYTHON)

if (BUILD_TESTING_CASES)
    include(utils/test/CMakeLists.txt)
    #include(YutilsCpp/test/CMakeLists.txt)
endif(BUILD_TESTING_CASES)
