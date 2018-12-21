#include <Bullet.hpp>
#include <KeyboardManager.hpp>
#include <Player.hpp>
#include <Term.hpp>
#include <algorithm>
#include <memory>

const double ACCELERATION = 0.5;
const double DECELERATION = ACCELERATION * 0.05;
const double MAX_SPEED = 3;
const int coolDown = 100;

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
    deltaX += DECELERATION;
    if(deltaX > 0){
      deltaX = 0;
    }
  } else {
    deltaX -= DECELERATION;
    if(deltaX < 0){
      deltaX = 0;
    }
  }

  if (deltaY < 0) {
    deltaY += DECELERATION;
    if(deltaY > 0){
      deltaY = 0;
    }
  } else {
    deltaY -= DECELERATION;
    if(deltaY < 0){
      deltaY = 0;
    }
  }
}

double Player::GetX() { return x_pos; }

double Player::GetY() { return y_pos; }

void Player::SetX(uint32_t x) { x_pos = x; }

void Player::SetY(uint32_t y) { y_pos = y; }

void Player::AddToDelta(double* delta, double add) {
  *delta += add;
  *delta = std::clamp(*delta, (double)-MAX_SPEED, (double)MAX_SPEED);
}

void Player::KeyPressed(KeyCode code) {
  switch (code) {
    case KeyCode::UP:
      AddToDelta(&deltaY, -ACCELERATION);
      break;
    case KeyCode::DOWN:
      AddToDelta(&deltaY, ACCELERATION);
      break;
    case KeyCode::LEFT:
      AddToDelta(&deltaX, -ACCELERATION);
      break;
    case KeyCode::RIGHT:
      AddToDelta(&deltaX, ACCELERATION);
      break;
    case KeyCode::SPACE:
      FireBullet();
      break;
  }
}

void Player::FireBullet() {
  std::shared_ptr<Drawable> bullet(new Bullet(term, entityManager, x_pos + 2, y_pos - 1));
  entityManager->AddEntity(bullet);
  auto a = system::time
  lastBulletFired = std::chrono::duration_cast<std::chrono::milliseconds>();
}
