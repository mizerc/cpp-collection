all: clean build run clean

build:
	mkdir -p build
	g++ `sdl2-config --libs --cflags` -std=c++17  -Wall -lm -o ./build/game ./src/*.cpp

run:
	./build/game 30 6 ./roms/IBM_Logo.ch8

clean:
	rm -rf build