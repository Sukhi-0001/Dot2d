#include "spdlog/spdlog.h"
#include <algorithm>
#include <ecs.hpp>
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
  int entity_id;
  if (free_ids.empty()) {
    entity_id = num_entities++;
    if (entity_id >= entity_component_signatures.size()) {
      entity_component_signatures.resize(entity_id + 1);
    }
  } else {
    // resuse id from free_ids stack
    entity_id = free_ids.front();
    free_ids.pop_front();
  }
  Entity entity(entity_id);
  entity.registry = this;
  if (entity_id >= entity_component_signatures.size()) {
    entity_component_signatures.resize(entity_id + 1);
  }
  entities_to_be_added.insert(entity);
  spdlog::info("Entity created with id {0:d}", entity_id);
  return entity;
}
void Entity::kill() { registry->kill_entity(*this); }

void Entity::tag(const std::string &tag) { registry->tag_entity(*this, tag); }
bool Entity::has_tag(const std::string &tag) {
  return registry->entity_has_tag(*this, tag);
}

void Entity::group(const std::string &group_name) {
  registry->group_entity(*this, group_name);
}
bool Entity::belong_to_group(const std::string &group_name) {
  return registry->entity_belongs_to_group(*this, group_name);
}

void Registry::kill_entity(Entity entity) {
  entities_to_be_killed.insert(entity);
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

void Registry::remove_entity_from_system(Entity entity) {
  for (auto system : systems) {
    system.second->remove_entity_from_system(entity);
  }
}

void Registry::update() {
  // add entities waiting to be added
  // spdlog::info("registry update component pool adder {}",
  //            component_pools.size());
  for (auto entity : entities_to_be_added) {
    add_entity_to_systems(entity);
  }
  entities_to_be_added.clear();
  // remove all the entities that are needed to be removed
  for (auto entity : entities_to_be_killed) {
    remove_entity_from_system(entity);
    // clear the component bitset also
    entity_component_signatures[entity.get_id()].reset();
    // make the entity to be reuse
    free_ids.push_back(entity.get_id());
    // remove tags and group
    remove_entity_tag(entity);
    remove_entity_group(entity);
  }
  entities_to_be_killed.clear();
}

// tag mangement
void Registry::tag_entity(Entity entity, const std::string &tag) {
  entity_per_tag.emplace(tag, entity);
  tag_per_entity.emplace(entity.get_id(), tag);
}
bool Registry::entity_has_tag(Entity entity, const std::string &tag) {
  if (tag_per_entity.find(entity.get_id()) == tag_per_entity.end()) {
    return false;
  }
  return entity_per_tag.find(tag)->second == entity;
}
Entity Registry::get_entity_by_tag(const std::string &tag) {
  return entity_per_tag.at(tag);
}
void Registry::remove_entity_tag(Entity entity) {
  auto tagged_entity = tag_per_entity.find(entity.get_id());
  if (tagged_entity != tag_per_entity.end()) {
    auto tag = tagged_entity->second;
    entity_per_tag.erase(tag);
    tag_per_entity.erase(tagged_entity);
  }
}
// group management
void Registry::group_entity(Entity entity, const std::string &group) {
  entities_per_group.emplace(group, std::set<Entity>());
  entities_per_group[group].emplace(entity);
  group_per_entity.emplace(entity.get_id(), group);
  spdlog::info("added group {0}", group);
}
bool Registry::entity_belongs_to_group(Entity entity,
                                       const std::string &group) {

  auto it = entities_per_group.find(group);
  if (it == entities_per_group.end()) {
    spdlog::warn("this group not found {0}", group);
    return false; // or handle error
  }
  auto &group_entities = it->second;
  return group_entities.find(entity.get_id()) != group_entities.end();
}
std::vector<Entity> Registry::get_entities_by_group(const std::string &group) {
  auto &set_of_entities = entities_per_group.at(group);
  return std::vector<Entity>(set_of_entities.begin(), set_of_entities.end());
}
void Registry::remove_entity_group(Entity entity) {
  auto grouped_entity = group_per_entity.find(entity.get_id());
  if (grouped_entity != group_per_entity.end()) {
    auto group = entities_per_group.find(grouped_entity->second);
    if (group != entities_per_group.end()) {
      auto entity_in_group = group->second.find(entity);
      if (entity_in_group != group->second.end()) {
        group->second.erase(entity_in_group);
      }
    }
    group_per_entity.erase(grouped_entity);
  }
}
