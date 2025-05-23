#ifndef ECS_H
#define ECS_H
#include <bitset>
#include <deque>
#include <memory>
#include <set>
#include <spdlog/spdlog.h>
#include <string>
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
  int id;
  // done
public:
  class Registry *registry;

  Entity(int id) : id(id) {};
  Entity(const Entity &other) = default;
  int get_id() const;
  void kill();

  Entity &operator=(const Entity &other) = default;
  bool operator==(const Entity &other) const { return id == other.id; }
  bool operator!=(const Entity &other) const { return id != other.id; }
  bool operator>(const Entity &other) const { return id > other.id; }
  bool operator<(const Entity &other) const { return id < other.id; }

  template <typename T, typename... Targs> void add_component(Targs &&...args);
  template <typename T> void remove_component();
  template <typename T> bool has_component() const;
  template <typename T> T &get_component() const;
  void tag(const std::string &tag);
  bool has_tag(const std::string &tag);

  void group(const std::string &group_name);
  bool belong_to_group(const std::string &group_name);
};

class Ipool {
public:
  virtual ~Ipool() = default;
  virtual void remove_entity_from_pool(int entity_it) = 0;
};
// POOL is just a wrapper around vector
template <typename T> class Pool : public Ipool {
  std::vector<T> data;
  int size;
  // helper keeps track of entity ids per index so the vector is packed
  std::unordered_map<int, int> entity_id_to_index;
  std::unordered_map<int, int> index_to_entity_id;

public:
  Pool(int capacity = 100) {
    size = 0;
    data.resize(capacity);
  }
  bool is_empty() const { return size == 0; }
  int get_size() const { return size; }
  void resize(int size) { data.resize(size); }
  void clear() {
    data.clear();
    size = 0;
  }
  void add(T object) { data.push_back(object); }
  void set(int entity_id, T object) {
    if (entity_id_to_index.find(entity_id) != entity_id_to_index.end()) {
      // if already exits replace it
      int index = entity_id_to_index[entity_id];
      data[index] = object;
    } else {
      int index = size;
      entity_id_to_index.emplace(entity_id, index);
      index_to_entity_id.emplace(index, entity_id);
      if (index >= data.capacity()) {
        data.resize(size * 2);
      }
      data[index] = object;
      size++;
    }
  }
  void remove(int entity_id) {
    // copy the last element to keep the data packed
    int index_of_removed = entity_id_to_index[entity_id];
    int index_of_last = size - 1;
    data[index_of_removed] = data[index_of_last];

    // update the index
    int entity_id_of_last_element = index_to_entity_id[index_of_last];
    entity_id_to_index[entity_id_of_last_element] = index_of_removed;
    index_to_entity_id[index_of_removed] = entity_id_of_last_element;

    entity_id_to_index.erase(entity_id);
    index_to_entity_id.erase(index_of_last);
    size--;
  }
  void remove_entity_from_pool(int entity_id) override {
    if (entity_id_to_index.find(entity_id) != entity_id_to_index.end()) {
      remove(entity_id);
    }
  }
  T &get(int entity_id) {
    int index = entity_id_to_index[entity_id];
    return static_cast<T &>(data[index]);
  }
  T &operator[](unsigned int index) { return data[index]; }
};
// done
class System {
  Signature components_signature;
  std::vector<Entity> entities;
  // TODO
public:
  System() = default;
  ~System() = default;
  void add_entity_to_system(Entity entity);
  void remove_entity_from_system(Entity entity);
  std::vector<Entity> get_system_entities() const;
  const Signature &get_components_signature() const;
  template <typename Tcomponent> void require_comopent();
};
// done
class Registry {
  int num_entities = 0;
  // one tag per entity
  std::unordered_map<int, std::string> tag_per_entity;
  std::unordered_map<std::string, Entity> entity_per_tag;
  // enities per group set of entites per group
  std::unordered_map<std::string, std::set<Entity>> entities_per_group;
  std::unordered_map<int, std::string> group_per_entity;
  // endtities to be added at end of frame or kill
  std::set<Entity> entities_to_be_added;
  std::set<Entity> entities_to_be_killed;

  // each pool contains all data of a Component type
  // vector index == component_id
  // pool index == enitity id
  std::vector<std::shared_ptr<Ipool>> component_pools;
  // vector of each components signature for Entity
  // vector index == Entity.id
  std::vector<Signature> entity_component_signatures;
  // map of active systems
  std::unordered_map<std::type_index, std::shared_ptr<System>> systems;
  // free ids are stored when entites are removed so it can be reused again
  std::deque<int> free_ids;

public:
  Registry() {}
  Entity create_entity();
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
  void remove_entity_from_system(Entity entity);
  // tag management
  void tag_entity(Entity entity, const std::string &tag);
  bool entity_has_tag(Entity entity, const std::string &tag);
  Entity get_entity_by_tag(const std::string &tag);
  void remove_entity_tag(Entity entity);
  // group management
  void group_entity(Entity entity, const std::string &group);
  bool entity_belongs_to_group(Entity entity, const std::string &group);
  std::vector<Entity> get_entities_by_group(const std::string &group);
  void remove_entity_group(Entity entity);
};
// done
template <typename T, typename... Targs>
void Registry::add_component(Entity entity, Targs &&...args) {
  const int component_id = Component<T>::get_id();
  const int entity_id = entity.get_id();

  if (component_id >= component_pools.size()) {
    component_pools.resize(component_id + 1, NULL);
  }

  if (!component_pools[component_id]) {
    std::shared_ptr<Pool<T>> new_component_pool(new Pool<T>());
    component_pools[component_id] = new_component_pool;
  }

  std::shared_ptr<Pool<T>> component_pool =
      std::static_pointer_cast<Pool<T>>(component_pools[component_id]);

  T new_component(std::forward<Targs>(args)...);

  component_pool->set(entity_id, new_component);
  entity_component_signatures[entity_id].set(component_id);
  spdlog::info("Added component id {0} to entity id {1} ", component_id,
               entity_id);
}

template <typename T> void Registry::remove_component(Entity entity) {

  const int component_id = Component<T>::get_id();
  const int entity_id = entity.get_id();

  entity_component_signatures[entity_id].set(component_id, false);
  std::shared_ptr<Pool<T>> component_pool =
      std::static_pointer_cast<Pool<T>>(component_pools[component_id]);
  component_pool->remove(entity_id);
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
// done
template <typename Tsystem, typename... Targs>
void Registry::add_system(Targs &&...args) {
  std::shared_ptr<Tsystem> new_system =
      std::make_shared<Tsystem>(std::forward<Targs>(args)...);
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
  return *(std::static_pointer_cast<Tsystem>(system->second));
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
