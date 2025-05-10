#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_joystick.h>
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>

#include "lcd.hpp"
#include "nodes/FunctionNode.hpp"
#include "nodes/GroupNode.hpp"
#include "nodes/LabelNode.hpp"

int main() {
  // Init LCD Panel
  LCD::init();
  LCD::clearAndSet("Loading", "");

  GroupNode *currentParentNode = nullptr;

  auto homeNode =
      std::make_unique<GroupNode>("Home", nullptr, &currentParentNode);

  auto musicGroupNode =
      std::make_unique<GroupNode>("Mus", homeNode.get(), &currentParentNode);
  {
    musicGroupNode->addNode(std::make_unique<FunctionNode>(
        "Toggle", musicGroupNode.get(), []() { system("mpc toggle"); }));

    musicGroupNode->addNode(std::make_unique<FunctionNode>(
        "Skip", musicGroupNode.get(), []() { system("mpc next"); }));
  }

  homeNode->addNode(std::move(musicGroupNode));
  homeNode->addNode(std::make_unique<LabelNode>("SMS", homeNode.get()));
  homeNode->addNode(std::make_unique<LabelNode>("RSS", homeNode.get()));
  homeNode->addNode(std::make_unique<LabelNode>("$Rec", homeNode.get()));

  currentParentNode = homeNode.get();
  currentParentNode->render();

  // Init our joystick
  if (SDL_Init(SDL_INIT_JOYSTICK) < 0) {
    std::cout << "Error: " << SDL_GetError() << "\n";
  }

  if (SDL_NumJoysticks() < 1) {
    printf("Warning: No joysticks connected!\n");
  } else {
    if (SDL_JoystickOpen(0) == NULL) {
      printf("Warning: Unable to open game controller! SDL Error: %s\n",
             SDL_GetError());
    }
  }

  while (true) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
      case SDL_JOYBUTTONDOWN:
        switch (e.jbutton.button) {
        case 0: {
          auto *node = currentParentNode->getSelectedNode();
          if (node) {
            node->action();
          } else {
            currentParentNode = homeNode.get();
          }
        }

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
