#include <EntityManager.hpp>
#include <Collideable.hpp>

EntityManager::EntityManager() {}

void EntityManager::AddEntity(std::shared_ptr<Drawable> entity) {
  std::lock_guard<std::mutex> lock(listMutex);
  entities.push_back(entity);
}

void EntityManager::RemoveEntity(std::shared_ptr<Drawable> entityToRemove) {
  std::lock_guard<std::mutex> lock(listMutex);
  entities.remove(entityToRemove);
}

void EntityManager::RemoveEntity(Drawable* entityToRemove) {
  std::lock_guard<std::mutex> lock(listMutex);
  for(auto it = entities.begin(); it != entities.end(); it++){
    if(it->get() == entityToRemove){
      entities.erase(it);
      break;
    }
  }
}

void EntityManager::DrawEntities() {

  // Copy the list to prevent the modifications from the draws to collide with the loop
  listMutex.lock();
  std::list<std::shared_ptr<Drawable>> list = entities;
  listMutex.unlock();

  for (auto a : list) {
    a->Draw();
  }
}

void EntityManager::UpdateEntities() {

  // Copy the list to prevent the modifications from the updates to with the readscollide with the loop
  listMutex.lock();
  std::list<std::shared_ptr<Drawable>> list;
  list.assign(entities.begin(), entities.end());
  listMutex.unlock();

  for (auto a : list) {
    a->Update();
  }
}
