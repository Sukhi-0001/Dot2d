#ifndef ECS_H
#define ECS_H
#include <bitset>
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
  //  TODO
  static int get_id() {
    static int id = next_id++;
    return id;
  }
};

class Entity {
  // TODO
  int id;

public:
  Entity(int id) : id(id) {};
  int get_id() const;
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
  // TODO
};

template <typename Tcomponent> void System::require_comopent() {
  const int component_id = Component<Tcomponent>::get_id();
  components_signature.set(component_id);
}
#endif // !ECS_H
