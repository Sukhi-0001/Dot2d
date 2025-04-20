#ifndef ECS_H
#define ECS_H
#include <bitset>
#include <vector>
const unsigned int MAX_COMPONENTS = 32;
typedef std::bitset<MAX_COMPONENTS> Signature;
class Component {
  int id;
  // TODO
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
std::vector<typename Tp>
    // TODO
    public : System() = default;
  void add_entity_to_system();
  void remove_entity_from_system();
  std::vector<Entity> get_system_entities() const;
  Signature &get_components_signature() const;
};

class Registry {
  // TODO
};
#endif // !ECS_H
