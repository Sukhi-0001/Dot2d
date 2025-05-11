#pragma once
#include <SDL2/SDL.h>
#include <map>
#include <string>
class Assets_manager {
public:
  Assets_manager();
  ~Assets_manager();

  void clear_assets();
  void add_texture(SDL_Renderer *renderer, const std::string &asset_id,
                   const std::string &file_path);
  SDL_Texture *get_texture(const std::string &asset_id);

private:
  std::map<std::string, SDL_Texture *> textures;
};
