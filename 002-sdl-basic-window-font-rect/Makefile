all: clean build run

build:
	mkdir -p build
	g++ `sdl2-config --libs --cflags` -lSDL2_ttf -std=c++17  -Wall -lm -o ./build/game ./src/*.cpp

run:
	./build/game 99 test

clean:
	rm -rf build