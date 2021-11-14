# - Try to find pcre2
# Once done, this will define
#
#  PCRE2_FOUND - system has pcre2
#  PCRE2_INCLUDE_DIRS - the pcre2 include directories
#  PCRE2_LIBRARIES - link these to use pcre2
#  PCRE2_DEFINITIONS - compiler flags

FIND_PACKAGE(PkgConfig)

# pango main library
PKG_CHECK_MODULES(PC_PCRE2 QUIET libpcre2-8)
SET(PCRE2_DEFINITIONS ${PC_PCRE2_CFLAGS_OTHER})

SET(PCRE2_INCLUDE_HINTS
    ${PC_PCRE2_INCLUDEDIR}
    ${PC_PCRE2_INCLUDE_DIRS}
    $ENV{PCRE2_HOME}/include
    $ENV{PCRE2_ROOT}/include
    ${PCRE2_ROOT_DIR}/include
    /usr/local/include
    /usr/include
    /pcre2/include
)

FIND_PATH(PCRE2_INCLUDE_DIR
    NAMES pcre2.h
    HINTS ${PCRE2_INCLUDE_HINTS}
)

SET(PCRE2_LIBRARY_HINTS
    ${PC_PCRE2_LIBDIR}
    ${PC_PCRE2_LIBRARY_DIRS}
    $ENV{PCRE2_HOME}/lib
    $ENV{PCRE2_ROOT}/lib
    ${PCRE2_ROOT_DIR}/lib
    /usr/local/lib
    /usr/lib
    /lib
    /pcre2/lib
)
FIND_LIBRARY(PCRE2_LIBRARY
    NAMES pcre2-8 libpcre2-8
    HINTS ${PCRE2_LIBRARY_HINTS}
)

SET(PCRE2_INCLUDE_DIRS ${PCRE2_INCLUDE_DIR})
SET(PCRE2_LIBRARIES ${PCRE2_LIBRARY})

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
    PCRE2
    DEFAULT_MSG
    PCRE2_INCLUDE_DIRS
    PCRE2_LIBRARIES
)

MARK_AS_ADVANCED(PANGO_INCLUDE_DIR PANGO_LIBRARY)
