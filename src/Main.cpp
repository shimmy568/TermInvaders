// print width and height
#include <unistd.h>

#include <signal.h>
#include <stdio.h>
#include <KeyboardManager.hpp>
#include <Player.hpp>
#include <Term.hpp>
#include <iostream>
#include <memory>
#include <thread>
#include <string>

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


  // term->SetCursorCol(10);
  // term->SetCursorRow(10);
  // term->PutString("hello world");
  // term->ClearTerm();
  // usleep(999999999999999);  

  //term->SetCursorVisible(false);

  KeyboardManager keyboardManager;
  std::shared_ptr<KeyboardManager> keyboardManagerPointer(&keyboardManager);

  EntityManager entityManager;
  std::shared_ptr<EntityManager> entityManagerPointer(&entityManager);

  Player p(term, keyboardManagerPointer, entityManagerPointer);

  p.SetX(10);
  p.SetY(10);

  while (running) {
    term->ClearTerm();

    entityManager.UpdateEntities();
    entityManager.DrawEntities();

    usleep(16666);
  }

  return 0;
}
