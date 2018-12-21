// print width and height
#include <unistd.h>

#include <signal.h>
#include <stdio.h>
#include <Enemy.hpp>
#include <KeyboardManager.hpp>
#include <Player.hpp>
#include <Term.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <chrono>

::std::shared_ptr<ts::Term> term;

bool_t running = true;

void my_handler(int s) {
  term->SetCursorVisible(true);
  exit(1);
}

int main(int argc, char **argv) {
  struct sigaction sigIntHandler;

  sigIntHandler.sa_handler = my_handler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;

  sigaction(SIGINT, &sigIntHandler, NULL);

  term = ts::Term::Create(STDOUT_FILENO);

  term->SetCursorVisible(false);

  KeyboardManager keyboardManager;
  std::shared_ptr<KeyboardManager> keyboardManagerPointer(&keyboardManager);

  EntityManager entityManager;
  std::shared_ptr<EntityManager> entityManagerPointer(&entityManager);

  Player p(term, keyboardManagerPointer, entityManagerPointer);

  Enemy test(term, entityManagerPointer, 10, 10);
  std::shared_ptr<Drawable> testPtr(&test);
  entityManager.AddEntity(testPtr);

  p.SetX(term->GetWidth() / 2);
  p.SetY(term->GetHeight() - 5);

  while (running) {
    auto start = std::chrono::high_resolution_clock::now();
    term->ClearTerm();

    entityManager.UpdateEntities();
    entityManager.DrawEntities();

    // Sleep for an amount of time that makes the current loop take 1/60th of a second
    auto delta = std::chrono::high_resolution_clock::now() - start;
    auto value = std::chrono::duration_cast<std::chrono::microseconds>(delta).count();
    if(16666 - value > 0){
      usleep(16666 - value > 0);
    }
  }

  return 0;
}
