#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_joystick.h>
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>

#include "nodes/GroupNode.hpp"
#include "nodes/LabelNode.hpp"

int main() {
  Node *currentParentNode = nullptr;

  auto homeNode =
      std::make_unique<GroupNode>("Home", nullptr, &currentParentNode);

  auto musicGroupNode =
      std::make_unique<GroupNode>("Mus", homeNode.get(), &currentParentNode);
  {
    musicGroupNode->addNode(
        std::make_unique<LabelNode>("Pause", musicGroupNode.get()));
    musicGroupNode->addNode(
        std::make_unique<LabelNode>("Play", musicGroupNode.get()));
    musicGroupNode->addNode(
        std::make_unique<LabelNode>("Skip", musicGroupNode.get()));
  }

  homeNode->addNode(std::move(musicGroupNode));
  homeNode->addNode(std::make_unique<LabelNode>("Two", homeNode.get()));
  homeNode->addNode(std::make_unique<LabelNode>("Trhee", homeNode.get()));
  homeNode->addNode(std::make_unique<LabelNode>("Four", homeNode.get()));
  homeNode->addNode(std::make_unique<LabelNode>("Five", homeNode.get()));

  currentParentNode = homeNode.get();
  currentParentNode->render();

  if (!SDL_Init(SDL_INIT_JOYSTICK)) {
    std::cout << "Error: " << SDL_GetError() << "\n";
  }

  if (!SDL_HasJoystick()) {
    throw std::logic_error("Joystick not connected");
  }

  SDL_JoystickID joystickID = SDL_GetJoysticks(NULL)[0];
  SDL_Joystick *joystick = SDL_OpenJoystick(joystickID);

  while (true) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
      case SDL_EVENT_JOYSTICK_BUTTON_DOWN:
        switch (e.jbutton.button) {
        case 0:
          currentParentNode->action();
          currentParentNode->render();
          break;
        case 1:
          currentParentNode->right();
          currentParentNode->render();
          break;
        case 2:
          currentParentNode->left();
          currentParentNode->render();
          break;
        }
        break;
      }
    }

    signal(SIGINT, [](int sig) { std::exit(1); });
  }
}
