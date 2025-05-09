#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_joystick.h>
#include <algorithm>
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

class Node {
  Node *parent;

public:
  std::string name;

  Node(std::string name, Node *parent) : name(name), parent(parent) {}
  virtual void action() {};
  virtual void render() {};
  virtual void right() {};
  virtual void left() {};
};

class GroupNode : public Node {
  std::vector<std::unique_ptr<Node>> subnodes;
  int selection = 0;

public:
  GroupNode(const std::string &name, Node *parent) : Node(name, parent) {}

  void addNode(std::unique_ptr<Node> node) {
    subnodes.push_back(std::move(node));
  }

  void action() override {
    // TODO
  }

  void render() override {
    std::string str;
    for (int i = 0; i < subnodes.size(); i++) {
      auto &subnodePtr = subnodes[i];

      if (selection == i) {
        str += "[" + subnodePtr->name + "]";
      } else {
        str += subnodePtr->name;
      }

      str += " ";
    }
    str += "...";

    std::cout << "\n\n";
    std::cout << str << "\n\n";
  }

  void right() override {
    selection += 1;
    if (selection >= subnodes.size()) {
      selection = 0;
    }
  }

  void left() override {
    selection -= 1;
    if (selection < 0) {
      selection = subnodes.size() - 1;
    }
  }
};

class LabelNode : public Node {
public:
  LabelNode(const std::string &name, Node *parent) : Node(name, parent) {}
  void action() override {}
};

int main() {
  std::unique_ptr<GroupNode> homeNode =
      std::make_unique<GroupNode>("", nullptr);
  homeNode->addNode(std::make_unique<LabelNode>("One", homeNode.get()));
  homeNode->addNode(std::make_unique<LabelNode>("Two", homeNode.get()));

  Node *currentParentNode = homeNode.get();
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
          currentParentNode->right();
          currentParentNode->render();
          break;
        case 1:
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
