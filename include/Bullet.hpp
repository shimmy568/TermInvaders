#pragma once

#include <Drawable.hpp>
#include <Term.hpp>
#include <EntityManager.hpp>

#include <memory>

class Bullet : public Drawable, Collideable {
    float x_pos;
    float y_pos;

    std::shared_ptr<ts::Term> term;
    std::shared_ptr<EntityManager> entityManager;
 public:
  void Draw();
  void Update();
  int getX();
  int getY();
  int getWidth();
  int getHeight();
  Bullet(std::shared_ptr<ts::Term> term, std::shared_ptr<EntityManager> entityManager, float x, float y);
};