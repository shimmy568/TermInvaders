#pragma once

#include <Drawable.hpp>
#include <Term.hpp>

#include <memory>

class Bullet : public Drawable {
    float x_pos;
    float y_pos;

    std::shared_ptr<ts::Term> term;
 public:
  void Draw();
  void Update();
  Bullet(std::shared_ptr<ts::Term> term, float x, float y);
};