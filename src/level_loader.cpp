#include "level_loader.hpp"
#include "SDL_render.h"
#include "ecs.hpp"
#include "glm/fwd.hpp"
#include "health_component.hpp"
#include "sprite_component.hpp"
#include "transform_component.hpp"
#include <Camera_movement_system.hpp>
#include <Projectile_life_cycle_system.hpp>
#include <SDL.h>
#include <animation_component.hpp>
#include <animation_system.hpp>
#include <box_collider_component.hpp>
#include <camera_follow_component.hpp>
#include <cmath>
#include <collision_system.hpp>
#include <cstdlib>
#include <damage_system.hpp>
#include <ecs.hpp>
#include <events/event_bus.hpp>
#include <fstream>
#include <game.hpp>
#include <glm/glm.hpp>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include <keyboard_control_component.hpp>
#include <keyboard_control_system.hpp>
#include <level_loader.hpp>
#include <movement_system.hpp>
#include <projectile_emit_system.hpp>
#include <projectile_emitter_component.hpp>
#include <render_collision_system.hpp>
#include <render_gui_system.hpp>
#include <render_health_bar_system.hpp>
#include <render_system.hpp>
#include <render_text_system.hpp>
#include <rigid_body_component.hpp>
#include <sol/error.hpp>
#include <sol/load_result.hpp>
#include <sol/optional_implementation.hpp>
#include <sol/sol.hpp>
#include <sol/state.hpp>
#include <sol/table.hpp>
#include <sol/types.hpp>
#include <spdlog/spdlog.h>
#include <sprite_component.hpp>
#include <string>
#include <text_label_component.hpp>
#include <transform_component.hpp>

Level_loader::Level_loader() {}

Level_loader::~Level_loader() {}

void Level_loader::load_level(
    sol::state &lua, const std::unique_ptr<Registry> &registry,
    const std::unique_ptr<Assets_manager> &assets_manager,
    SDL_Renderer *renderer, int level_no) {

  sol::load_result script = lua.load_file("../assets/scripts/Level" +
                                          std::to_string(level_no) + ".lua");
  if (!script.valid()) {
    sol::error err = script;
    std::string err_msg = err.what();
    spdlog::warn("{0}", err_msg);
    return;
  }
  lua.script_file("../assets/scripts/Level" + std::to_string(level_no) +
                  ".lua");
  sol::table level = lua["Level"];
  // read the level assets
  sol::table assets = level["assets"];
  int i = 0;
  while (true) {
    sol::optional<sol::table> has_asset = assets[i];
    if (has_asset == sol::nullopt) {
      break;
    }
    sol::table asset = assets[i];
    std::string asset_type = asset["type"];
    if (asset_type == "texture") {
      assets_manager->add_texture(renderer, asset["id"], asset["file"]);
      std::string asset_id = asset["id"];
      // spdlog::info("A new texture added into assets manager id {0}",
      // asset_id);
    }
    if (asset_type == "font") {
      assets_manager->add_font(asset["id"], asset["file"], asset["font_size"]);
      std::string asset_id = asset["id"];
      // spdlog::info("A new font added into assets manager id {0}",asset_id);
    }
    i++;
  }

  // Read the tilemap from file

  sol::table map = level["tilemap"];
  std::string map_file_path = map["map_file"];
  std::string map_texture_asset_id = map["texture_asset_id"];

  int tile_size = map["tile_size"];
  double map_scale = map["scale"];
  int map_cols = map["num_cols"];
  int map_rows = map["num_rows"];
  std::fstream map_file;
  map_file.open(map_file_path);
  for (int y = 0; y < map_cols; y++) {
    for (int x = 0; x < map_rows; x++) {
      char ch;
      map_file.get(ch);
      int src_rect_y = std::atoi(&ch) * tile_size;
      map_file.get(ch);
      int src_rect_x = std::atoi(&ch) * tile_size;
      map_file.ignore();
      Entity tile = registry->create_entity();
      // tile.group("group");
      tile.add_component<Transform_component>(
          glm::vec2(x * (map_scale * tile_size), y * (map_scale * tile_size)),
          glm::vec2(map_scale, map_scale), 0);
      tile.add_component<Sprite_component>(map_texture_asset_id, tile_size,
                                           tile_size, 0, false, src_rect_x,
                                           src_rect_y);
    }
  }
  map_file.close();
  Game::map_width = map_cols * tile_size * map_scale;
  Game::map_height = map_cols * tile_size * map_scale;

  /*
  int tile_size = 32;
  double tile_scale = 2.0;
  int map_cols = 25;
  int map_rows = 20;
  std::fstream map_file;
  map_file.open("../assets/tilemaps/jungle.map");
  for (int y = 0; y < map_cols; y++) {
    for (int x = 0; x < map_rows; x++) {
      char ch;
      map_file.get(ch);
      int src_rect_y = std::atoi(&ch) * tile_size;
      map_file.get(ch);
      int src_rect_x = std::atoi(&ch) * tile_size;
      map_file.ignore();
      Entity tile = registry->create_entity();
      tile.group("group");
      tile.add_component<Transform_component>(
          glm::vec2(x * (tile_scale * tile_size), y * (tile_scale * tile_size)),
          glm::vec2(tile_scale, tile_scale), 0);
      tile.add_component<Sprite_component>("tilemap-texture", tile_size,
                                           tile_size, 0, false, src_rect_x,
                                           src_rect_y);
    }
  }
  map_file.close();
  Game::map_width = map_cols * tile_size * tile_scale;
  Game::map_height = map_cols * tile_size * tile_scale;
  // adding assets to manger
  /*
    assets_manager->add_texture(renderer, "tank-img",
                                "../assets/images/tank-panther-up.png");
    assets_manager->add_texture(renderer, "tilemap-img",
                                "../assets/tilemaps/jungle.png");
    assets_manager->add_texture(renderer, "chopper-img",
                                "../assets/images/chopper-spritesheet.png");
    assets_manager->add_texture(renderer, "bullet-img",
                                "../assets/images/bullet.png");
    assets_manager->add_font("charriot-font", "../assets/fonts/charriot.ttf",
    14); assets_manager->add_font("pico8-font-5", "../assets/fonts/arial.ttf",
    5);
    // todo load tilemap

    int tile_size = 32;
    double tile_scale = 2.0;
    int map_cols = 25;
    int map_rows = 20;
    std::fstream map_file;
    map_file.open("../assets/tilemaps/jungle.map");
    for (int y = 0; y < map_cols; y++) {
      for (int x = 0; x < map_rows; x++) {
        char ch;
        map_file.get(ch);
        int src_rect_y = std::atoi(&ch) * tile_size;
        map_file.get(ch);
        int src_rect_x = std::atoi(&ch) * tile_size;
        map_file.ignore();
        Entity tile = registry->create_entity();
        tile.group("group");
        tile.add_component<Transform_component>(
            glm::vec2(x * (tile_scale * tile_size), y * (tile_scale *
    tile_size)), glm::vec2(tile_scale, tile_scale), 0);
        tile.add_component<Sprite_component>("tilemap-img", tile_size,
    tile_size, 0, false, src_rect_x, src_rect_y);
      }
    }
    map_file.close();
    Game::map_width = map_cols * tile_size * tile_scale;
    Game::map_height = map_cols * tile_size * tile_scale;
    Entity tank = registry->create_entity();
    tank.group("enimies");
    tank.add_component<Rigid_body_component>(glm::vec2(10, 0));
    tank.add_component<Transform_component>(glm::vec2(10, 10), glm::vec2(2, 1),
                                            0.0);
    tank.add_component<Sprite_component>("tank-img", 32, 32, 1);
    tank.add_component<Box_collider_component>(32, 32);
    tank.add_component<Health_component>(50);
    tank.add_component<Projectile_emitter_component>(glm::vec2(150, 150), 1000,
                                                     1000, 10, false);

    Entity chopper = registry->create_entity();
    chopper.tag("player");
    chopper.add_component<Sprite_component>("chopper-img", 32, 32, 1);
    chopper.add_component<Animation_component>(2, 24, true);
    chopper.add_component<Transform_component>(glm::vec2(100, 0), glm::vec2(2,
    1), 0.0); chopper.add_component<Box_collider_component>(32, 32);
    chopper.add_component<Rigid_body_component>(glm::vec2(20, 0));
    chopper.add_component<Keyboard_control_component>(
        glm::vec2(0, -20), glm::vec2(20, 0), glm::vec2(0, 20), glm::vec2(-20,
    0)); chopper.add_component<Camera_follow_component>();
    chopper.add_component<Projectile_emitter_component>(glm::vec2(150, 150), 0,
                                                        10000, 10, true);
    chopper.add_component<Health_component>(100);

    Entity label = registry->create_entity();
    SDL_Color text_color = {255, 0, 0};
    label.add_component<Text_label_component>(
        glm::vec2(Game::window_width / 2 - 40, 10), "CHOPPER 0.1",
        "charriot-font", text_color, true);
    */
}
