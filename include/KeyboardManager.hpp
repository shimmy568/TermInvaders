#pragma once

#include <KeyboardListener.hpp>
#include <list>
#include <mutex>
#include <memory>

class KeyboardManager {
  std::list<std::shared_ptr<KeyboardListener>> listeners;

  bool IsListenerRunning;

  void KeyPressed(KeyCode code);

  void ResetAllKeys();

  char getch();
  void ListenForChars();

 public:
  KeyboardManager();

  void AddListener(std::shared_ptr<KeyboardListener> listener);

  void StopListener();
};