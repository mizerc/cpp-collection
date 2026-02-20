all: clean build run

build:
	mkdir -p build
	g++ -I/opt/homebrew/opt/gmp/include \
	-L/opt/homebrew/opt/gmp/lib \
	-lgmp -lm -std=c++17 -Wall  \
	-o ./build/fib ./src/fib.cpp

run:
	./build/fib 1000000

clean:
	rm -rf build