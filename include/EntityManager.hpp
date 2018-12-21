#pragma once

#include <Collideable.hpp>
#include <Drawable.hpp>
#include <list>
#include <memory>
#include <mutex>
#include <vector>

class EntityManager {
  std::list<std::shared_ptr<Drawable>> entities;

  std::mutex listMutex;

 public:
  EntityManager();

  void AddEntity(std::shared_ptr<Drawable> entity);
  void RemoveEntity(std::shared_ptr<Drawable> entityToRemove);
  void RemoveEntity(Drawable* entityToRemove);

  void DrawEntities();
  void UpdateEntities();

  // Get all the entities that collided with c that are of a given type
  template <typename T>
  std::vector<std::shared_ptr<T>> findCollided(Collideable c) {
    std::vector<std::shared_ptr<T>> output;
    for (auto p : entities) {
      if (std::shared_ptr<Collideable> np = std::dynamic_pointer_cast<Collideable>(p)) {
        Collideable ct = *np;
        if (&ct == &c) {
          continue;
        }

        if (std::shared_ptr<T> nnp = std::dynamic_pointer_cast<T>(p)) {
          if (np->isCollided(c)) {
            output.push_back(nnp);
          }
        }
      }
    }

    return output;
  }
};