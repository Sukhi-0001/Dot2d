#include <game.h>
void GAME::init() {}
void GAME::setup() {}

void GAME::process_input() {}
void GAME::update() {}
void GAME::render() {}
void GAME::run() {
  setup();
  while (is_running) {
    process_input();
    update();
    render();
  }
  return;
}
