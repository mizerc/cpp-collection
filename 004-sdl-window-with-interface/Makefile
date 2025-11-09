CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 `sdl2-config --cflags`
LDFLAGS := `sdl2-config --libs` -lm
SRC_DIR := src
BUILD_DIR := build
BIN_FILE := $(BUILD_DIR)/game
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_FILES))

.PHONY: all build run clean

all: build run

build: $(BIN_FILE)

$(BIN_FILE): $(OBJ_FILES)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(OBJ_FILES) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: build
	$(BIN_FILE)

clean:
	rm -rf $(BUILD_DIR)