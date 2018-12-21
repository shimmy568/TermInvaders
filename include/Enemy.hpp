#pragma once

#include <Collideable.hpp>
#include <Drawable.hpp>
#include <EntityManager.hpp>
#include <Term.hpp>
#include <memory>

class Enemy : public Drawable, Collideable {
  float x_pos;
  float y_pos;
  std::shared_ptr<EntityManager> entityManager;
  std::shared_ptr<ts::Term> term;

  bool goingLeft;

 public:
  void Draw();
  void Update();
  void Explode();
  int getX();
  int getY();
  int getWidth();
  int getHeight();
  Enemy(std::shared_ptr<ts::Term> term,
        std::shared_ptr<EntityManager> entityManager, float x_pos, float y_pos);
};
