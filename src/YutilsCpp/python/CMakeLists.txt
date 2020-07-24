find_package(pybind11 2.3.0 REQUIRED)
find_package(Python 3.8.0 REQUIRED COMPONENTS Interpreter Development)

add_library(${CMAKE_PROJECT_NAME}_YutilsPy SHARED
    yutilspy.cpp
)

add_dependencies(${CMAKE_PROJECT_NAME}_YutilsPy ${CMAKE_PROJECT_NAME}YutilsCpp)

#python
LIST(APPEND YutilsPy_includes ${PYTHON_INCLUDE_DIRS})
LIST(APPEND YutilsPy_includes ${pybind11_INCLUDE_DIRS})
LIST(APPEND YutilsPy_libs ${PYTHON_LIBRARIES})

target_link_libraries(${CMAKE_PROJECT_NAME}_YutilsPy
    ${YutilsPy_libs}
    ${CMAKE_PROJECT_NAME}YutilsCpp
)

target_include_directories(${CMAKE_PROJECT_NAME}_YutilsPy
    SYSTEM BEFORE
    PUBLIC
    ${YutilsPy_includes}
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${Boost_INCLUDE_DIRS}>
)

set_target_properties(${CMAKE_PROJECT_NAME}_YutilsPy PROPERTIES PREFIX "")

if (WIN32)
    set_target_properties(${CMAKE_PROJECT_NAME}_YutilsPy PROPERTIES SUFFIX ".pyd")
endif (WIN32)

set(SETUP_PY_IN "${CMAKE_CURRENT_SOURCE_DIR}/setup.py.in")
set(SETUP_PY    "${CMAKE_CURRENT_BINARY_DIR}/setup.py")
set(OUTPUT      "${CMAKE_CURRENT_BINARY_DIR}/timestamp")
set(DEPS        ${CMAKE_PROJECT_NAME}_YutilsPy "${SETUP_PY}")

configure_file(${SETUP_PY_IN} ${SETUP_PY} @ONLY)
configure_file(assPy.py ${CMAKE_PROJECT_NAME}_AssPy.py @ONLY)

add_custom_command(OUTPUT ${OUTPUT}
                   COMMAND
                   ${Python_EXECUTABLE} ${SETUP_PY} build -b
                   "${CMAKE_CURRENT_SOURCE_DIR}"

                   COMMAND
                   ${Python_EXECUTABLE} ${SETUP_PY} egg_info -b
                   "${CMAKE_CURRENT_SOURCE_DIR}"

                   COMMAND ${CMAKE_COMMAND} -E touch ${OUTPUT}
                   DEPENDS ${DEPS})

add_custom_target(python_bindings ALL DEPENDS ${OUTPUT})

execute_process(
    COMMAND "${Python_EXECUTABLE}" -c
    "from distutils.sysconfig import get_python_lib; print(get_python_lib(prefix='', plat_specific=True))"
    OUTPUT_VARIABLE PYTHON_SITE_PACKAGES
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

if ("${PYTHON_MODULE_INSTALL_PREFIX}" STREQUAL "")
    install(
        TARGETS ${CMAKE_PROJECT_NAME}_YutilsPy

        DESTINATION "${PYTHON_SITE_PACKAGES}"
    )

    install(
        DIRECTORY
        "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}_YutilsPy.egg-info"

        DESTINATION "${PYTHON_SITE_PACKAGES}"
    )

    install(
        FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}_AssPy.py"

        DESTINATION "${PYTHON_SITE_PACKAGES}"
    )

else()
    install(
        TARGETS ${CMAKE_PROJECT_NAME}_YutilsPy

        DESTINATION "${PYTHON_MODULE_INSTALL_PREFIX}/${PYTHON_SITE_PACKAGES}"
    )

    install(
        DIRECTORY
        "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}_YutilsPy.egg-info"

        DESTINATION "${PYTHON_MODULE_INSTALL_PREFIX}/${PYTHON_SITE_PACKAGES}"
    )

    install(
        FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}_AssPy.py"

        DESTINATION "${PYTHON_MODULE_INSTALL_PREFIX}/${PYTHON_SITE_PACKAGES}"
    )
endif("${PYTHON_MODULE_INSTALL_PREFIX}" STREQUAL "")
