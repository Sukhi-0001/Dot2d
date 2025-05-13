#pragma once
#include "event.hpp"
#include <SDL2/SDL.h>
class Key_press_event : public Event {
public:
  SDL_Keycode symbol;
  Key_press_event(SDL_Keycode symbol) : symbol(symbol) {}
};
