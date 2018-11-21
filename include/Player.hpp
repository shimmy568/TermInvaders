#pragma once

#include <memory>

#include <Drawable.hpp>
#include <KeyboardListener.hpp>
#include <EntityManager.hpp>
#include <Term.hpp>

class Player : public Drawable, public KeyboardListener {
  float x_pos, y_pos;

  float deltaX, deltaY;

  ::std::shared_ptr<ts::Term> term;
  std::shared_ptr<EntityManager> entityManager;

  void AddToDelta(float* delta, float add);

  void FireBullet();

 public:
  Player(::std::shared_ptr<ts::Term>,
         ::std::shared_ptr<KeyboardManager> keyboardManager, ::std::shared_ptr<EntityManager> entityManager);

  void Draw();
  void Update();

  float GetX();
  float GetY();

  void SetX(uint32_t);
  void SetY(uint32_t);

  void KeyPressed(KeyCode code);
  
};
