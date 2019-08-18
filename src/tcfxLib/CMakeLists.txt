add_library(tcfxLib SHARED
        std.hpp
        pix.hpp
        pix.cpp
        file.hpp
        file.cpp
        text.hpp
        text.cpp
        image.hpp
        image.cpp
        utility.hpp
        utility.cpp
        python/tcfxlib.cpp)

#boost
LIST(APPEND tcfxLib_includes ${Boost_INCLUDE_DIRS})
LIST(APPEND tcfxLib_libs ${Boost_PYTHON3_LIBRARY} ${Boost_LOCALE_LIBRARY})

#python
LIST(APPEND tcfxLib_includes ${Python_INCLUDE_DIRS})
LIST(APPEND tcfxLib_libs ${Python_LIBRARIES})

#freetype2
LIST(APPEND tcfxLib_includes ${FREETYPE_INCLUDE_DIRS})
LIST(APPEND tcfxLib_libs ${FREETYPE_LIBRARIES})

#libpng
find_package(PNG 1.6.36 REQUIRED)
LIST(APPEND tcfxLib_includes ${PNG_INCLUDE_DIRS})
LIST(APPEND tcfxLib_libs ${PNG_LIBRARIES})

target_link_libraries(tcfxLib ${tcfxLib_libs})
target_include_directories(tcfxLib PUBLIC ${tcfxLib_includes})
set_target_properties(tcfxLib PROPERTIES PREFIX "")
