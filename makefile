CXX=g++
CC=gcc
IDIR=include/
CXXF=-pg
CCF=-pg


all:

test:
	echo $@
clean:
	rm -rf ./build

builddir:
	mkdir -p build

TaskManager: builddir src/TaskManager.cpp include/TaskManager.hpp
	$(CXX) $(CXXF) -o build/$@.o
