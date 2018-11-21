#include <Bullet.hpp>

Bullet::Bullet(std::shared_ptr<ts::Term> term, float x, float y)
    : term(term), x_pos(x), y_pos(y) {}

void Bullet::Draw() {
  term->SetCursorCol(x_pos);
  term->SetCursorRow(y_pos);
  term->SetBackgroundColor(ts::TermColor::RED);
  term->PutChar(' ');
}

void Bullet::Update() { 
  y_pos -= 1;
}