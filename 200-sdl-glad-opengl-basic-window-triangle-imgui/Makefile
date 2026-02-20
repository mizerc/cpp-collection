all: clean build run

build:
	mkdir -p build
	g++ \
	 -Iinclude \
	 `sdl2-config --libs --cflags` \
	 -std=c++17  \
	 -Wall \
	 -lm \
	 -o ./build/game \
	 ./src/*.cpp \
	 ./src/imgui/*.cpp \
	 ./src/glad/glad.c

run:
	./build/game 99 test

clean:
	rm -rf build