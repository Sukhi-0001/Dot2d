#include "spdlog/spdlog.h"
#include <algorithm>
#include <ecs.h>
#include <vector>
int Base_component::next_id = 0;

int Entity::get_id() const { return id; }

void System::add_entity_to_system(Entity entity) {
  entities.push_back(entity);
  return;
}

void System::remove_entity_from_system(Entity entity) {
  entities.erase(std::remove_if(entities.begin(), entities.end(),
                                [&entity](Entity other) {
                                  return other.get_id() == entity.get_id();
                                }),
                 entities.end());
}

std::vector<Entity> System::get_system_entities() const { return entities; }

const Signature &System ::get_components_signature() const {
  return components_signature;
}

// Registry
Entity Registry::create_entity() {
  int entity_id = num_entities++;
  Entity entity(entity_id);
  entity.registry = this;
  if (entity_id >= entity_component_signatures.size()) {
    entity_component_signatures.resize(entity_id + 1);
  }
  entities_to_be_added.insert(entity);
  spdlog::info("Entity created with id {0:d}", entity_id);
  return entity;
}

void Registry::add_entity_to_systems(Entity entity) {
  int entity_id = entity.get_id();
  const auto entity_component_signature =
      entity_component_signatures[entity_id];

  for (auto &system : systems) {
    const auto &system_component_signature =
        system.second->get_components_signature();
    bool is_intreseted =
        (entity_component_signature & system_component_signature) ==
        system_component_signature;
    if (is_intreseted) {
      system.second->add_entity_to_system(entity);
    }
  }
}

void Registry::update() {
  // add entities waiting to be added
  spdlog::info("registry update component pool adder {}",
               component_pools.size());
  for (auto entity : entities_to_be_added) {
    add_entity_to_systems(entity);
  }
  entities_to_be_added.clear();
}
