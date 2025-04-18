#include <game.h>
#include <glm/vec3.hpp>
using namespace std;

int main() {

  Game game;
  game.init();
  game.run();
  game.destroy();
  return 0;
}
