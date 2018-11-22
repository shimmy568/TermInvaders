#pragma once

#include <Drawable.hpp>
#include <Term.hpp>
#include <EntityManager.hpp>

#include <memory>

class Bullet : public Drawable {
    float x_pos;
    float y_pos;

    std::shared_ptr<ts::Term> term;
    std::shared_ptr<EntityManager> entityManager;
 public:
  void Draw();
  void Update();
  Bullet(std::shared_ptr<ts::Term> term, std::shared_ptr<EntityManager> entityManager, float x, float y);
};