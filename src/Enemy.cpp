#include <Enemy.hpp>
#include <string>
#include <vector>

const double SPEED = 1;
const double DOWN_AMOUNT = 5;
const std::vector<std::string> body{" x   x ", "xxxxxxx", "xxx xxx", "xx   xx"};
const int height = body.size();
const int width = body[0].length();
const int sideBuffer = 5;

Enemy::Enemy(std::shared_ptr<ts::Term> term,
             std::shared_ptr<EntityManager> entityManager, float x_pos,
             float y_pos)
    : goingLeft(false),
      term(term),
      entityManager(entityManager),
      x_pos(x_pos),
      y_pos(y_pos) {}

void Enemy::Draw() {
  term->SetCursorRow(y_pos);
  term->SetCursorCol(x_pos);
  term->SetForgroundColor(ts::TermColor::GREEN);
  term->SetBackgroundColor(ts::TermColor::DEFAULT);

  for (auto s : body) {
    term->PutString(s);
    term->MoveCursorDown(1);
    term->SetCursorCol(x_pos);
  }
}

int Enemy::getX() { return x_pos; }

int Enemy::getY() { return y_pos; }

int Enemy::getWidth() { return width; }

int Enemy::getHeight() { return height; }

void Enemy::Update() {
  if (goingLeft) {
    x_pos -= SPEED;
  } else {
    x_pos += SPEED;
  }

  if ((x_pos < sideBuffer && goingLeft) ||
      (x_pos > term->GetWidth() - (width + sideBuffer) && !goingLeft)) {
    goingLeft = !goingLeft;
    y_pos += DOWN_AMOUNT;
  }
}
