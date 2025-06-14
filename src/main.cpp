#include <game.hpp>
#include <glm/vec3.hpp>
#include <sol/sol.hpp>
using namespace std;

int main() {

  Game game;
  game.init();
  game.run();
  game.destroy();

  return 0;
}
