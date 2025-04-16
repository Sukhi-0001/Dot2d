#ifndef GAME_H
#define GAME_H
#include <SDL.h>

class GAME {
private:
  bool is_running;
  SDL_Window *window;
  SDL_Renderer *renderer;

public:
  GAME() {}
  void init();
  void setup();
  void process_input();
  void update();
  void render();
  void run();
};

#endif // !GAME
