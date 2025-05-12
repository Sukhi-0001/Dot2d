#pragma once
#include "events/event.hpp"
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <typeindex>
#include <utility>

class Ievent_callback {
  virtual void call(Event &e) = 0;

public:
  virtual ~Ievent_callback() = default;

  void execute(Event &e) { call(e); }
};

template <typename TOwner, typename TEvent>
class Event_callback : public Ievent_callback {
  typedef void (TOwner::*Callback_function)(TEvent &);
  TOwner *owner_instance;
  Callback_function callback_function;
  virtual void call(Event &e) {
    std::invoke(callback_function, owner_instance, static_cast<TEvent &>(e));
  }

public:
  Event_callback(TOwner *owner_instance, Callback_function callback_function) {
    this->owner_instance = owner_instance;
    this->callback_function = callback_function;
  }
  virtual ~Event_callback() override = default;
};

typedef std::list<std::unique_ptr<Ievent_callback>> Hander_list;

class Event_bus {
  std::map<std::type_index, std::unique_ptr<Hander_list>> subscribers;

public:
  Event_bus() = default;
  ~Event_bus() = default;
  template <typename TEvent, typename TOwner>
  void subscribe_to_event(TOwner *owner_instance,
                          void (TOwner::*callback_function)(TEvent &)) {
    if (!subscribers[typeid(TEvent)].get()) {
      subscribers[typeid(TEvent)] = std::make_unique<Hander_list>();
    }
    auto subscriber = std::make_unique<Event_callback<TOwner, TEvent>>(
        owner_instance, callback_function);
    subscribers[typeid(TEvent)]->push_back(std::move(subscriber));
  }
  template <typename TEvent, typename... TArgs>
  void emit_event(TArgs &&...args) {
    auto handers = subscribers[typeid(TEvent)].get();
    if (handers) {
      for (auto it = handers->begin(); it != handers->end(); it++) {
        auto hander = it->get();
        TEvent event(std::forward<TArgs>(args)...);
        hander->execute(event);
      }
    }
  }

  void reset() { subscribers.clear(); }

private:
};
