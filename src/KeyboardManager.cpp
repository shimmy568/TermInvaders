#include <KeyboardManager.hpp>

#include <termios.h>
#include <unistd.h>
#include <thread>

KeyboardManager::KeyboardManager() {
  IsListenerRunning = true;
  std::thread listenerThread(&KeyboardManager::ListenForChars, this);
  listenerThread.detach();
}

void KeyboardManager::ListenForChars() {
  char c = 0;
  while (IsListenerRunning) {
    c = getch();

    // Arrow keys detection
    if (c == 27) {
      char c2 = getch();  // Remove the [ key code
      char c1 = getch();
      switch (c1) {
        case 'A':
          // Up
          KeyPressed(KeyCode::UP);
          break;
        case 'B':
          // Down
          KeyPressed(KeyCode::DOWN);
          break;
        case 'C':
          // Right
          KeyPressed(KeyCode::RIGHT);
          break;
        case 'D':
          // Left
          KeyPressed(KeyCode::LEFT);
          break;
      }
    } else if (c == ' ') {
      KeyPressed(KeyCode::SPACE);
    }
  }
}

char KeyboardManager::getch() {
  char buf = 0;
  struct termios old = {0};
  if (tcgetattr(0, &old) < 0) perror("tcsetattr()");
  old.c_lflag &= ~ICANON;
  old.c_lflag &= ~ECHO;
  old.c_cc[VMIN] = 1;
  old.c_cc[VTIME] = 0;
  if (tcsetattr(0, TCSANOW, &old) < 0) perror("tcsetattr ICANON");
  if (read(0, &buf, 1) < 0) perror("read()");
  old.c_lflag |= ICANON;
  old.c_lflag |= ECHO;
  if (tcsetattr(0, TCSADRAIN, &old) < 0) perror("tcsetattr ~ICANON");
  return (buf);
}

void KeyboardManager::AddListener(std::shared_ptr<KeyboardListener> listener) {
  listeners.push_back(listener);
}

void KeyboardManager::StopListener() { IsListenerRunning = false; }

void KeyboardManager::KeyPressed(KeyCode code) {
  for (auto a : listeners) {
    a->KeyPressed(code);
  }
}