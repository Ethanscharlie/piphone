#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "Node.hpp"

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
    int indexAtSelection = 0;

    for (int i = 0; i < subnodes.size(); i++) {
      auto &subnodePtr = subnodes[i];

      if (selection == i) {
        indexAtSelection = str.size();
        str += "[" + subnodePtr->name + "]";
      } else {
        str += subnodePtr->name;
      }

      str += " ";
    }
    str += "...";

    std::string outputStr = str.substr(indexAtSelection, 16);

    std::cout << "\n\n";
    std::cout << outputStr << "\n\n";
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
