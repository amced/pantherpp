CXX=g++
CC=gcc

all:

clean:
	rm -rf ./build

builddir:
	mkdir -p build

TaskQueue: builddir src/TaskQueue.cpp include/TaskManager.hpp
	$(CXX) $(CXXF) -g -c src/$@.cpp -o build/$@.o

Task: builddir src/Task.cpp include/TaskManager.hpp
	$(CXX) $(CXXF) -g -c src/$@.cpp -o build/$@.o

fsutil: builddir src/fsutil.cpp include/fsutil.hpp
	$(CXX) $(CXXF) -g -c src/$@.cpp -o build/$@.o

StaticController: builddir src/StaticController.cpp include/TaskManager.hpp
	$(CXX) $(CXXF) -g -c src/$@.cpp -o build/$@.o
