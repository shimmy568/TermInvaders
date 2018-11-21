#include <Bullet.hpp>
#include <KeyboardManager.hpp>
#include <Player.hpp>
#include <Term.hpp>
#include <algorithm>
#include <memory>

Player::Player(::std::shared_ptr<ts::Term> term,
               ::std::shared_ptr<KeyboardManager> keyboardManager,
               ::std::shared_ptr<EntityManager> entityManager)
    : x_pos(0), y_pos(0), deltaX(0), deltaY(0) {
  this->term = term;
  std::shared_ptr<KeyboardListener> keyboardListenerPointer(this);
  keyboardManager->AddListener(keyboardListenerPointer);

  std::shared_ptr<Drawable> drawablePointer(this);
  entityManager->AddEntity(drawablePointer);

  this->entityManager = entityManager;
}

void Player::Draw() {
  term->SetCursorCol(x_pos);
  term->SetCursorRow(y_pos);

  // Print gun
  term->SetBackgroundColor(ts::TermColor::DEFAULT);
  term->PutString("  ");
  term->SetBackgroundColor(ts::TermColor::RED);
  term->PutString(" ");
  term->SetBackgroundColor(ts::TermColor::DEFAULT);
  term->PutString("  ");
  term->MoveCursorDown(1);
  term->MoveCursorLeft(5);

  term->SetBackgroundColor(ts::TermColor::GREEN);
  for (auto i = 0; i < 2; i++) {
    term->PutString("     ");
    term->MoveCursorDown(1);
    term->MoveCursorLeft(5);
  }
}

void Player::Update() {
  x_pos += deltaX;
  y_pos += deltaY;
  if (deltaX < 0) {
  } else {
  }
}

float Player::GetX() { return x_pos; }

float Player::GetY() { return y_pos; }

void Player::SetX(uint32_t x) { x_pos = x; }

void Player::SetY(uint32_t y) { y_pos = y; }

void Player::AddToDelta(float* delta, float add) {
  *delta += add;
  *delta = std::clamp(*delta, (float)-3, (float)-3.0);
}

void Player::KeyPressed(KeyCode code) {
  switch (code) {
    case KeyCode::UP:
      AddToDelta(&y_pos, -0.2);
      break;
    case KeyCode::DOWN:
      AddToDelta(&y_pos, 0.2);
      break;
    case KeyCode::LEFT:
      AddToDelta(&x_pos, -0.2);
      break;
    case KeyCode::RIGHT:
      AddToDelta(&x_pos, 0.2);
      break;
    case KeyCode::SPACE:
      FireBullet();
      break;
  }
}

void Player::FireBullet() {
  std::shared_ptr<Drawable> bullet(new Bullet(term, x_pos + 2, y_pos - 1));
  entityManager->AddEntity(bullet);
}
