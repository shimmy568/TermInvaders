#pragma once

#include <Drawable.hpp>
#include <list>
#include <memory>
#include <mutex>

class EntityManager {
  std::list<std::shared_ptr<Drawable>> entities;

  std::mutex listMutex;

 public:
  EntityManager();
  
  void AddEntity(std::shared_ptr<Drawable> entity);
  void RemoveEntity(std::shared_ptr<Drawable> entityToRemove);

  void DrawEntities();
  void UpdateEntities();
};