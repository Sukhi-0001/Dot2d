#include <game.hpp>
#include <glm/vec3.hpp>
#include <sol/sol.hpp>
using namespace std;

int main() {
  sol::state lua;
  int x = 0;
  lua.set_function("beep", [&x] { ++x; });
  lua.script("beep()");
  assert(x == 1);

  Game game;
  game.init();
  game.run();
  game.destroy();

  return 0;
}
