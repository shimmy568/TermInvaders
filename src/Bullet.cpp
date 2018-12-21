#include <Bullet.hpp>
#include <Enemy.hpp>

Bullet::Bullet(std::shared_ptr<ts::Term> term,
               std::shared_ptr<EntityManager> entityManager, float x, float y)
    : term(term), entityManager(entityManager), x_pos(x), y_pos(y) {}

void Bullet::Draw() {
  term->SetCursorCol(x_pos);
  term->SetCursorRow(y_pos);
  term->SetBackgroundColor(ts::TermColor::RED);
  term->SetForgroundColor(ts::TermColor::DEFAULT);
  term->PutChar(' ');
}

int Bullet::getX() { return x_pos; }

int Bullet::getY() { return y_pos; }

int Bullet::getWidth() { return 1; }

int Bullet::getHeight() { return 1; }

void Bullet::Update() {
  y_pos -= 1;
  if (y_pos < 0) {
    entityManager->RemoveEntity(this);
  }

  auto vec = entityManager->findCollided<Enemy>(*this);

  for (auto en : vec) {
    entityManager->RemoveEntity(en);
    entityManager->RemoveEntity(this);
  }
}