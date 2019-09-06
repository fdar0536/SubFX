CXX = clang++
CLANG ?= clang++
CXXFLAG = -std=c++14 -Wall -Wextra -Werror -fPIC
LD = -shared -Wl,--unresolved-symbols=report-all
BUILD_TYPE ?= debug

ifeq ($(BUILD_TYPE), debug)
	CXXFLAG += -O0 -g
else
	CXXFLAG += -O3
endif

PANGO = $(shell pkgconf -cflags pangocairo)
PANGO_LD = $(shell pkgconf --libs pangocairo)
PYTHON = $(shell pkgconf -cflags python3)
PYTHON_LD = $(shell pkgconf --libs python3)
ICU_LD = $(shell pkgconf --libs icu-io)
BOOST_LD = -lboost_regex

OBJS = math.o common.o corelib.o fonthandle.o \
		ass.o file.o assparser.o assparserpy.o \
		utf8.o shape.o

JSONS = corelib.o.json common.o.json math.o.json \
		fonthandle.o.json ass.o.json file.o.json \
		assparser.o.json assparserpy.o.json utf8.o.json \
		shape.o.json

CoreLib.so: $(OBJS)
	$(CXX) $(CXXFLAG) $(PANGO_LD) $(PYTHON_LD) $(ICU_LD) $(BOOST_LD) $(LD) -o CoreLib.so $(OBJS)
	mv CoreLib.so test

compile_commands: $(JSONS)
	sed -e '1s/^/[\n/' -e '$$s/,\$$/\n]/' $(JSONS) > compile_commands.json

# corelib
corelib.o: python/corelib.cpp
	$(CXX) $(CXXFLAG) $(PYTHON) $(PANGO) -c python/corelib.cpp

corelib.o.json: python/corelib.cpp
	$(CLANG) -MJ corelib.o.json $(CXXFLAG) $(PYTHON) $(PANGO) -c python/corelib.cpp

# common
common.o: common.cpp
	$(CXX) $(CXXFLAG) -c common.cpp

common.o.json: common.cpp
	$(CLANG) -MJ common.o.json $(CXXFLAG) -c common.cpp

# math
math.o: math.cpp
	$(CXX) $(CXXFLAG) -c math.cpp

math.o.json: math.cpp
	$(CLANG) -MJ math.o.json $(CXXFLAG) -c math.cpp

# fonthandle
fonthandle.o: fonthandle.cpp
	$(CXX) $(CXXFLAG) $(PANGO) -c fonthandle.cpp

fonthandle.o.json: fonthandle.cpp
	$(CLANG) -MJ fonthandle.o.json $(CXXFLAG) $(PANGO) -c fonthandle.cpp

# ass
ass.o: ass.cpp
	$(CXX) $(CXXFLAG) -c ass.cpp

ass.o.json: ass.cpp
	$(CLANG) -MJ ass.o.json $(CXXFLAG) -c ass.cpp

# file
file.o: file.cpp
	$(CXX) $(CXXFLAG) -c file.cpp

file.o.json: file.cpp
	$(CLANG) -MJ file.o.json $(CXXFLAG) -c file.cpp

# assparser
assparser.o: assparser.cpp
	$(CXX) $(CXXFLAG) $(PANGO) -c assparser.cpp

assparser.o.json: assparser.cpp
	$(CLANG) -MJ assparser.o.json $(CXXFLAG) $(PANGO) -c assparser.cpp

# assparserpy
assparserpy.o: python/assparserpy.cpp
	$(CXX) $(CXXFLAG) -c python/assparserpy.cpp

assparserpy.o.json: python/assparserpy.cpp
	$(CLANG) -MJ assparserpy.o.json $(CXXFLAG) -c python/assparserpy.cpp

# utf8
utf8.o: utf8.cpp
	$(CXX) $(CXXFLAG) -c utf8.cpp

utf8.o.json: utf8.cpp
	$(CLANG) -MJ utf8.o.json $(CXXFLAG) -c utf8.cpp

# shape
shape.o: shape.cpp
	$(CXX) $(CXXFLAG) -c shape.cpp

shape.o.json: shape.cpp
	$(CLANG) -MJ shape.o.json $(CXXFLAG) -c shape.cpp

# clean up
.phony clean:
	rm -rf $(OBJS) $(JSONS) *.o

distclean: clean
	rm -rf compile_commands.json
	rm -rf test/*.so
	rm -rf testCpp

# testing
test: CoreLib.so
	python test/test.py

test.o: test/test.cpp
	$(CXX) $(CXXFLAG) -c test/test.cpp

testCpp: test.o common.o math.o shape.o utf8.o
	$(CXX) $(CXXFLAG) -Wl,--unresolved-symbols=report-all -o testCpp common.o math.o shape.o utf8.o test.o

test_cpp: testCpp
	./testCpp