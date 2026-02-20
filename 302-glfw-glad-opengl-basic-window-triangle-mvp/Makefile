all: clean build run

clean:
	rm -rf build

build:
	mkdir -p build
	g++ \
	src/glad.c \
	src/main.cpp \
	-std=c++17 \
	-Iinclude \
	$(shell pkg-config --cflags --libs glfw3) \
	-ldl \
	-o build/main

run:
	./build/main