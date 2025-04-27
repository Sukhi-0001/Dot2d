#ifndef ECS_H
#define ECS_H
#include "spdlog/spdlog.h"
#include <bitset>
#include <memory>
#include <set>
#include <typeindex>
#include <unordered_map>
#include <utility>
#include <vector>
const unsigned int MAX_COMPONENTS = 32;
typedef std::bitset<MAX_COMPONENTS> Signature;

struct Base_component {
protected:
  static int next_id;
};
// used to assign unique id to per component type
template <typename T> class Component : public Base_component {
  // int id;
public:
  static int get_id() {
    static int id = next_id++;
    return id;
  }
};

class Entity {
  // TODO
  int id;

public:
  class Registry *registry;

  Entity(int id) : id(id) {};
  Entity(const Entity &other) = default;
  int get_id() const;

  Entity &operator=(const Entity &other) = default;
  bool operator==(const Entity &other) const { return id == other.id; }
  bool operator!=(const Entity &other) const { return id != other.id; }
  bool operator>(const Entity &other) const { return id > other.id; }
  bool operator<(const Entity &other) const { return id < other.id; }

  template <typename T, typename... Targs> void add_component(Targs &&...args);
  template <typename T> void remove_component();
  template <typename T> bool has_component() const;
  template <typename T> T &get_component() const;
};

class Ipool {
public:
  virtual ~Ipool() {}
};
// POOL is just a wrapper around vector
template <typename T> class Pool : public Ipool {
  std::vector<T> data;

public:
  Pool(int size = 100) { data.resize(100); }
  bool is_empty() const { return data.empty(); }
  int get_size() const { return data.size(); }
  void resize(int size) { data.resize(size); }
  void clear() { data.clear(); }
  void add_object(T object) { data.push_back(object); }
  void set(int index, T object) { data[index] = object; }
  T &get(int index) { return static_cast<T &>(data[index]); }
  T &operator[](unsigned int index) { return data[index]; }
};

class System {
  Signature components_signature;
  std::vector<Entity> entities;
  // TODO
public:
  System() = default;
  void add_entity_to_system(Entity entity);
  void remove_entity_from_system(Entity entity);
  std::vector<Entity> get_system_entities() const;
  const Signature &get_components_signature() const;
  template <typename Tcomponent> void require_comopent();
};

class Registry {
  int num_entities = 0;
  // endtities to be added at end of frame or kill
  std::set<Entity> entities_to_be_added;
  std::set<Entity> entities_to_be_killed;

  // each pool contains all data of a Component type
  // vector index == component_id
  // pool index == enitity id
  std::vector<Ipool *> component_pools;
  // vector of each components signature for Entity
  // vector index == Entity.id
  std::vector<Signature> entity_component_signatures;
  // map of active systems
  std::pmr::unordered_map<std::type_index, System *> systems;

  // TODO
public:
  Registry() {}
  Entity create_entity();
  void add_entity_to_system(Entity entity);
  void kill_entity(Entity entity);
  void update();
  template <typename T, typename... Targs>
  void add_component(Entity entity, Targs &&...args);
  template <typename T> void remove_component(Entity entity);
  template <typename T> bool has_component(Entity entity) const;
  template <typename T> T &get_component(Entity entity) const;

  template <typename Tsystem, typename... Targs>
  void add_system(Targs &&...args);
  template <typename Tsystem> void remove_system();
  template <typename Tsystem> bool has_system() const;
  template <typename Tsystem> Tsystem &get_system() const;
  // checks the components signature of the entity and add entity to the system
  // that are interseted in it
  void add_entity_to_systems(Entity entity);
};

template <typename T, typename... Targs>
void Registry::add_component(Entity entity, Targs &&...args) {
  const int component_id = Component<T>::get_id();
  const int entity_id = entity.get_id();

  if (component_id >= component_pools.size()) {
    component_pools.resize(component_id + 1, NULL);
  }

  if (!component_pools[component_id]) {
    Pool<T> *new_component_pool = new Pool<T>();
    component_pools[component_id] = new_component_pool;
  }

  Pool<T> *component_pool =
      static_cast<Pool<T> *>(component_pools[component_id]);

  if (entity_id >= component_pool->get_size()) {
    component_pool->resize(num_entities);
  }

  T new_component(std::forward<Targs>(args)...);

  component_pool->set(entity_id, new_component);

  entity_component_signatures[entity_id].set(component_id);
  spdlog::info("Added component id ${0} to entity id ${1} ", component_id,
               entity_id);
}

template <typename T> void Registry::remove_component(Entity entity) {

  const int component_id = Component<T>::get_id();
  const int entity_id = entity.get_id();

  entity_component_signatures[entity_id].set(component_id, false);
  spdlog::info("Removed component {0} from entity {1}", component_id,
               entity_id);
}

template <typename T> bool Registry::has_component(Entity entity) const {

  const int component_id = Component<T>::get_id();
  const int entity_id = entity.get_id();
  return entity_component_signatures[entity_id].test(component_id);
}
template <typename T> T &Registry::get_component(Entity entity) const {
  const int component_id = Component<T>::get_id();
  const int entity_id = entity.get_id();
  auto component_pool =
      std::static_pointer_cast<Pool<T>>(component_pools[component_id]);
  return component_pool->get(entity_id);
}

template <typename Tsystem, typename... Targs>
void Registry::add_system(Targs &&...args) {
  Tsystem *new_system(new Tsystem(std::forward<Targs>(args)...));
  systems.insert(std::make_pair(std::type_index(typeid(Tsystem)), new_system));
}
template <typename Tsystem> void Registry::remove_system() {
  auto system = systems.find(std::type_index(typeid(Tsystem)));
  systems.erase(system);
}
template <typename Tsystem> bool Registry::has_system() const {
  return systems.find(std::type_index(typeid(Tsystem))) != systems.end();
}
template <typename Tsystem> Tsystem &Registry::get_system() const {

  auto system = systems.find(std::type_index(typeid(Tsystem)));
  return std::static_pointer_cast<Tsystem>(system->second);
}

template <typename Tcomponent> void System::require_comopent() {
  const int component_id = Component<Tcomponent>::get_id();
  components_signature.set(component_id);
}

template <typename T, typename... Targs>
void Entity::add_component(Targs &&...args) {
  registry->add_component<T>(*this, std::forward<Targs>(args)...);
}
template <typename T> void Entity::remove_component() {
  registry->remove_component<T>(*this);
}
template <typename T> bool Entity::has_component() const {
  return registry->has_component<T>(*this);
}
template <typename T> T &Entity::get_component() const {
  return registry->get_component<T>(*this);
}
#endif // !ECS_H
