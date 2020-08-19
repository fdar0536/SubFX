cmake_minimum_required(VERSION 3.14)

project(SubFX
    LANGUAGES CXX
    VERSION 0.0.2
)

set(CMAKE_CXX_STANDARD 14)
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

option(ENABLE_SIMD "Enable X86 SIMD, include SSE" ON)
if(ENABLE_SIMD)
    option(ENABLE_AVX "Enable AVX" ON)
endif(ENABLE_SIMD)

if (UNIX)
    add_compile_options(-Wall -Werror -Wextra -fvisibility=hidden -fvisibility-inlines-hidden)

    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        add_compile_options(-Weffc++)
    endif ()

    if(ENABLE_SIMD)
        add_compile_options(-msse)
        if (ENABLE_AVX)
            add_compile_options(-mavx)
        endif (ENABLE_AVX)
    endif(ENABLE_SIMD)

    add_link_options(-Wl,--unresolved-symbols=report-all)
else () # Windows
    if (MINGW)
        add_compile_options(-Wall -Werror -Wextra -fvisibility=hidden -fvisibility-inlines-hidden)

        if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
            add_compile_options(-Weffc++)
        endif ()

        if(ENABLE_SIMD)
            add_compile_options(-msse)
            if (ENABLE_AVX)
                add_compile_options(-mavx)
            endif (ENABLE_AVX)
        endif(ENABLE_SIMD)

        add_link_options(-Wl,--unresolved-symbols=report-all)
    else () # msvc or icc
        add_compile_options(/W4 /WX /wd4819)
        if(ENABLE_SIMD)
            if (ENABLE_AVX)
                add_compile_options(/arch:AVX)
            endif (ENABLE_AVX)
        endif(ENABLE_SIMD)
    endif (MINGW)
endif (UNIX) # end if (UNIX)

enable_testing()

#common
add_library(BaseCommon
    INTERFACE
)

target_include_directories(BaseCommon
    INTERFACE
    include/internal
    ${CMAKE_CURRENT_BINARY_DIR}
)

set_target_properties(BaseCommon
    PROPERTIES
    PUBLIC_HEADER
    "include/internal/basecommon.h;\
${CMAKE_BINARY_DIR}/config.h"
    )

install(
    TARGETS BaseCommon

    PUBLIC_HEADER
    DESTINATION
    ${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_PROJECT_NAME}/internal
    COMPONENT
    devel
)

install(
    EXPORT ${CMAKE_PROJECT_NAME}Targets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${CMAKE_PROJECT_NAME}
    FILE ${CMAKE_PROJECT_NAME}Targets.cmake
    NAMESPACE ${CMAKE_PROJECT_NAME}::
    COMPONENT devel
)

add_subdirectory(utils)

# python
option(ENABLE_PYTHON "Enable python binding" ON)
if (ENABLE_PYTHON)
    find_package(pybind11 2.3.0 REQUIRED)
    find_package(Python 3.7.0 REQUIRED COMPONENTS Interpreter Development)
endif(ENABLE_PYTHON)

add_subdirectory(YutilsCpp)

configure_file(config.h.in config.h @ONLY)

# frontend
if (ENABLE_PYTHON)
    include(SubFX/CMakeLists.txt)
endif (ENABLE_PYTHON)
