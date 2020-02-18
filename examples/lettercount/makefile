CXX=g++
CFLAGS= -pthread -lboost_random

EMXX=em++
EMXXFLAGS=-std=c++11 -s FETCH=1 -s EXTRA_EXPORTED_RUNTIME_METHODS=['lengthBytesUTF8','stringToUTF8','UTF8ToString'] -O3


all: client server

clean:
	rm server a.out.js a.out.wasm

client: ./client.cpp
	$(EMXX) ./$@.cpp $(EMXXFLAGS)

server: ./server.cpp ../../src/TaskQueue.cpp ../../src/Task.cpp ../../src/StaticController.cpp ../../src/fsutil.cpp
	$(CXX) ./$@.cpp ../../src/TaskQueue.cpp ../../src/Task.cpp ../../src/StaticController.cpp ../../src/fsutil.cpp $(CFLAGS) -o server