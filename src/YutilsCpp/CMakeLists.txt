

set(YutilsCpp_main_headers
    ${CMAKE_SOURCE_DIR}/include/internal/yutils.h
)

set(YutilsCpp_headers
    ${CMAKE_SOURCE_DIR}/include/internal/yutilscpp/ass.h
    ${CMAKE_SOURCE_DIR}/include/internal/yutilscpp/asscommon.h
    # ${YUTILSCPP_INCLUDE_PREFIX}/assparser.hpp
    # assparser.cpp
    # ${YUTILSCPP_INCLUDE_PREFIX}/asswriter.hpp
    # asswriter.cpp
    # internal/asswriter_internal.hpp
    # internal/asswriter_internal.cpp
    ${CMAKE_SOURCE_DIR}/include/internal/yutilscpp/fonthandle.h
    ${CMAKE_SOURCE_DIR}/include/internal/yutilscpp/math.h
    # ${YUTILSCPP_INCLUDE_PREFIX}/matrix.hpp
    # matrix.cpp
    # ${YUTILSCPP_INCLUDE_PREFIX}/shape.hpp
    # shape.cpp
    # internal/shape_internal.hpp
    # internal/shape_internal.cpp
)

set(YutilsCpp_priv_headers
    YutilsCpp/ass.h
    # YutilsCpp/asscommon.h
    YutilsCpp/fonthandle.h
    YutilsCpp/init.h
    YutilsCpp/math.h
)

set(YutilsCpp_src
    YutilsCpp/ass.cpp
    # YutilsCpp/asscommon.c
    YutilsCpp/init.c
    YutilsCpp/math.c
    YutilsCpp/fonthandle.cpp
)
