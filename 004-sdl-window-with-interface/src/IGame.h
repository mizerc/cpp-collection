#pragma once
#include <SDL.h>

class IGame {
public:
    virtual ~IGame() = default;
    virtual void setup() = 0;
    virtual void handle_input(const SDL_Event& event) = 0;
    virtual void update(float dt) = 0;
    virtual void render() = 0;
};
