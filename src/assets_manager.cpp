#include "SDL_image.h"
#include "SDL_render.h"
#include "SDL_surface.h"
#include "SDL_ttf.h"
#include "spdlog/spdlog.h"
#include <assets_manager.hpp>

Assets_manager::Assets_manager() { spdlog::info("creating assets_manager"); }

Assets_manager::~Assets_manager() {
  clear_assets();
  spdlog::info("assets_manager destroyed");
}

void Assets_manager::clear_assets() {
  for (auto texture : textures) {
    SDL_DestroyTexture(texture.second);
  }
  textures.clear();
  // todo clear fonts
  for (auto font : fonts) {
    TTF_CloseFont(font.second);
  }
  fonts.clear();
}
void Assets_manager::add_texture(SDL_Renderer *renderer,
                                 const std::string &asset_id,
                                 const std::string &file_path) {
  SDL_Surface *surface = IMG_Load(file_path.c_str());
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
  SDL_FreeSurface(surface);
  // add texture to map
  textures.emplace(asset_id, texture);
  spdlog::info("new asset texture added asset id {0}", asset_id);
}
SDL_Texture *Assets_manager::get_texture(const std::string &asset_id) {
  if (textures.find(asset_id) == textures.end()) {
    spdlog::warn("Trying to accces asset that does not exits {0}", asset_id);
  }
  return textures[asset_id];
}

void Assets_manager::add_font(const std::string &asset_id,
                              const std::string &file_path, int font_size) {
  fonts.emplace(asset_id, TTF_OpenFont(file_path.c_str(), font_size));
  spdlog::info("new asset font added asset id {0}", asset_id);
}
TTF_Font *Assets_manager::get_font(const std::string &asset_id) {
  return fonts[asset_id];
}
