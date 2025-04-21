#include <algorithm>
#include <ecs.h>
#include <vector>

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
