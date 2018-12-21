#include <Collideable.hpp>

bool Collideable::isCollided(Collideable c) {
  if (c.getX() + c.getWidth() > this->getX() &&
      c.getY() + c.getHeight() > this->getY() &&
      this->getX() + this->getWidth() > c.getX() &&
      this->getY() + this->getHeight() > c.getY()) {
    return true;
  }
  return false;
}