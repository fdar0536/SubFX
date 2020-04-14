# This file is modified from libosmscout(https://github.com/Framstag/libosmscout)
#
# - Try to find Pango
# Once done, this will define
#
#  PANGO_FOUND - system has Pango
#  PANGO_INCLUDE_DIRS - the Pango include directories
#  PANGO_LIBRARIES - link these to use Pango
#  PANGO_DEFINITIONS - compiler flags
#  PANGOCAIRO_FOUND - system has Pangocairo

FIND_PACKAGE(PkgConfig)

# pango main library
PKG_CHECK_MODULES(PC_PANGO QUIET pango)
SET(PANGO_DEFINITIONS ${PC_PANGO_CFLAGS_OTHER})

SET(PANGO_INCLUDE_HINTS
    ${PC_PANGO_INCLUDEDIR}
    ${PC_PANGO_INCLUDE_DIRS}
    $ENV{PANGO_HOME}/include
    $ENV{PANGO_ROOT}/include
    /usr/local/include
    /usr/include
    /pango/include
)
FIND_PATH(PANGO_INCLUDE_DIR
    NAMES pango/pango.h
    HINTS ${PANGO_INCLUDE_HINTS}
    PATH_SUFFIXES pango pango-1.0 pango1.0
)

SET(PANGO_LIBRARY_HINTS
    ${PC_PANGO_LIBDIR}
    ${PC_PANGO_LIBRARY_DIRS}
    $ENV{PANGO_HOME}/lib
    $ENV{PANGO_ROOT}/lib
    /usr/local/lib
    /usr/lib
    /lib
    /pango/lib
)
FIND_LIBRARY(PANGO_LIBRARY
    NAMES pango PANGO PANGO-1.0 pango-1.0
    HINTS ${PANGO_LIBRARY_HINTS}
    PATH_SUFFIXES pango pango-1.0 pango1.0
)

SET(PANGO_INCLUDE_DIRS ${PANGO_INCLUDE_DIR})
SET(PANGO_LIBRARIES ${PANGO_LIBRARY})

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
    Pango
    DEFAULT_MSG
    PANGO_INCLUDE_DIRS
    PANGO_LIBRARIES
)

IF(PANGO_FOUND)
      # dependencies
      FIND_PACKAGE(GLIB2 REQUIRED)
      IF(GLIB2_FOUND)
          SET(PANGO_INCLUDE_DIRS ${PANGO_INCLUDE_DIRS} ${GLIB2_INCLUDE_DIRS})
          SET(PANGO_LIBRARIES ${PANGO_LIBRARIES} ${GLIB2_LIBRARIES})
      ENDIF()

      FIND_PACKAGE(GObject REQUIRED)
      IF(GLIB2_FOUND)
          SET(PANGO_INCLUDE_DIRS ${PANGO_INCLUDE_DIRS} ${GOBJECT_INCLUDE_DIRS})
          SET(PANGO_LIBRARIES ${PANGO_LIBRARIES} ${GOBJECT_LIBRARIES})
      ENDIF()

    # pangocairo
    PKG_CHECK_MODULES(PC_PANGOCAIRO QUIET pangocairo)
    FIND_PATH(PANGOCAIRO_INCLUDE_DIRS
        NAMES pango/pangocairo.h
        HINTS ${PC_PANGOCAIRO_INCLUDEDIR}
              ${PC_PANGOCAIRO_INCLUDE_DIRS}
              $ENV{PANGOCAIRO_HOME}/include
              $ENV{PANGOCAIRO_ROOT}/include
              /pangocairo/include
              ${PANGO_INCLUDE_HINTS}
        PATH_SUFFIXES pango pango-1.0 pangocairo libpangocairo-1.0 pangocairo1.0
    )

    IF(PANGOCAIRO_INCLUDE_DIRS)
        SET(PANGO_INCLUDE_DIRS ${PANGO_INCLUDE_DIRS} ${PANGOCAIRO_INCLUDE_DIRS})
    ENDIF()

    FIND_LIBRARY(PANGOCAIRO_LIBRARIES
        NAMES pangocairo PANGOcairo PANGOcairo-1.0 pangocairo-1.0 libpangocairo-1.0
        HINTS ${PC_PANGOCAIRO_LIBDIR}
              ${PC_PANGOCAIRO_LIBRARY_DIRS}
              $ENV{PANGOCAIRO_HOME}/lib
              $ENV{PANGOCAIRO_ROOT}/lib
              ${PANGO_LIBRARY_HINTS}
        PATH_SUFFIXES pango pango-1.0 pangocairo libpangocairo-1.0 pangocairo1.0
    )

    IF(PANGOCAIRO_LIBRARIES)
        SET(PANGO_LIBRARIES ${PANGO_LIBRARIES} ${PANGOCAIRO_LIBRARIES})
    ENDIF()

    FIND_PACKAGE_HANDLE_STANDARD_ARGS(
        Pangocairo
        DEFAULT_MSG
        PANGOCAIRO_INCLUDE_DIRS
        PANGOCAIRO_LIBRARIES
    )
ENDIF()
