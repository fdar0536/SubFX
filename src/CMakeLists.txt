cmake_minimum_required(VERSION 3.12)

project(SubFX VERSION 0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)

if (UNIX)
    add_compile_options(-Wall -Werror -Wextra)
    
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        add_compile_options(-Weffc++)
    endif ()
    
    add_link_options(-Wl,--unresolved-symbols=report-all)
else () # Windows
    if (MINGW)
        add_compile_options(-Wall -Werror -Wextra)
        
        if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
            add_compile_options(-Weffc++)
        endif ()
        
        add_link_options(-Wl,--unresolved-symbols=report-all)
    else () # msvc or icc
        add_compile_options(/W4 /WX)
    endif (MINGW)
endif (UNIX) # end if (UNIX)

if (UNIX)
    find_package(Boost 1.69.0 REQUIRED COMPONENTS regex)
else () # windows
    find_package(Boost 1.69.0 REQUIRED COMPONENTS regex locale)
endif (UNIX)

find_package(ICU 64.2 REQUIRED COMPONENTS uc)

option(ENABLE_SSE2 "Enable SSE2 SIMD" ON)
configure_file(config.h.in config.h @ONLY)
include_directories(After SYSTEM ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(CoreLib)
