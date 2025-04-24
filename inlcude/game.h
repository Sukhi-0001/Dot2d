#ifndef GAME_H
#define GAME_H
#include <SDL.h>
#include <ecs.h>

const int FPS = 60;
const int MILLISECONDS_PER_FPS = 1000 / FPS;

class Game {
private:
  bool is_running;
  int milliseconds_previous_frame;
  SDL_Window *window;
  SDL_Renderer *renderer;
  Registry *registry;

public:
  Game() {
    is_running = false;
    registry = new Registry();
  }
  void init();
  void setup();
  void process_input();
  void update();
  void render();
  void run();
  void destroy();
};

#endif // !GAME
