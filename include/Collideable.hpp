#pragma once

class Collideable {
 public:
  virtual int getX() {}
  virtual int getY() {}
  virtual int getWidth() {}
  virtual int getHeight() {}
  bool isCollided(Collideable c);
};