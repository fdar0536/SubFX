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

set(YUTILSCPP_INCLUDE_PREFIX ${CMAKE_SOURCE_DIR}/include/internal/yutilscpp)

set(${CMAKE_PROJECT_NAME}Utils_DIR "${CMAKE_BINARY_DIR}/utils")
find_package(${CMAKE_PROJECT_NAME}Utils CONFIG REQUIRED)

add_library(${CMAKE_PROJECT_NAME}YutilsCpp SHARED
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
    ${YUTILSCPP_INCLUDE_PREFIX}/math.h
    math.c
    internal/math_internal.h
    internal/math_internal.c
    # ${YUTILSCPP_INCLUDE_PREFIX}/matrix.hpp
    # matrix.cpp
    # ${YUTILSCPP_INCLUDE_PREFIX}/shape.hpp
    # shape.cpp
    # internal/shape_internal.hpp
    # internal/shape_internal.cpp
    ${CMAKE_SOURCE_DIR}/include/YutilsCpp
    ${CMAKE_SOURCE_DIR}/include/internal/yutils.h
)

add_library(${CMAKE_PROJECT_NAME}::Yutils ALIAS ${CMAKE_PROJECT_NAME}YutilsCpp)
file(WRITE
    ${CMAKE_BINARY_DIR}/YutilsCpp/${CMAKE_PROJECT_NAME}YutilsCppConfig.cmake
    "")

add_dependencies(${CMAKE_PROJECT_NAME}YutilsCpp BaseCommon)
LIST(APPEND YutilsCpp_libs ${CMAKE_PROJECT_NAME}::Utils)

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

target_link_libraries(${CMAKE_PROJECT_NAME}YutilsCpp PUBLIC ${YutilsCpp_libs})
target_include_directories(${CMAKE_PROJECT_NAME}YutilsCpp
    SYSTEM BEFORE
    PUBLIC
    ${YutilsCpp_includes}
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${Boost_INCLUDE_DIRS}>
)

#install
set_target_properties(${CMAKE_PROJECT_NAME}YutilsCpp
    PROPERTIES

    EXPORT_NAME Yutils

    VERSION ${PROJECT_VERSION}

    PUBLIC_HEADER
    "${CMAKE_SOURCE_DIR}/include/YutilsCpp"

    PRIVATE_HEADER
    "${YUTILSCPP_INCLUDE_PREFIX}/ass.hpp;\
${YUTILSCPP_INCLUDE_PREFIX}/asscommon.hpp;\
${YUTILSCPP_INCLUDE_PREFIX}/assparser.hpp;\
${YUTILSCPP_INCLUDE_PREFIX}/asswriter.hpp;\
${YUTILSCPP_INCLUDE_PREFIX}/fonthandle.hpp;\
${YUTILSCPP_INCLUDE_PREFIX}/math.h;\
${YUTILSCPP_INCLUDE_PREFIX}/matrix.hpp;\
${YUTILSCPP_INCLUDE_PREFIX}/shape.hpp"
)

install(
    TARGETS ${CMAKE_PROJECT_NAME}YutilsCpp
    DESTINATION
    ${CMAKE_INSTALL_LIBDIR}

    EXPORT ${CMAKE_PROJECT_NAME}Targets
    DESTINATION
    ${CMAKE_INSTALL_LIBDIR}

    LIBRARY
    DESTINATION
    ${CMAKE_INSTALL_LIBDIR}

    RUNTIME
    DESTINATION
    ${CMAKE_INSTALL_BINDIR}
    COMPONENT
    runtime

    ARCHIVE
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT
    devel

    PUBLIC_HEADER
    DESTINATION
    ${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_PROJECT_NAME}
    COMPONENT
    devel

    PRIVATE_HEADER
    DESTINATION
    ${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_PROJECT_NAME}/internal/yutilscpp
    COMPONENT
    devel
)

option(BUILD_TESTING_CASES "Build testing cases" ON)

# if (ENABLE_PYTHON)
#     add_subdirectory(python)
# endif (ENABLE_PYTHON)

# if (BUILD_TESTING_CASES)
#     include(test/CMakeLists.txt)
# endif (BUILD_TESTING_CASES)
