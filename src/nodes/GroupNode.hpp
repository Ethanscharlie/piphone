#pragma once
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "Node.hpp"
#include "lcd.hpp"

class GroupNode : public Node {
  std::vector<std::unique_ptr<Node>> subnodes;
  int selection = 0;
  GroupNode **currentParentNode;

public:
  GroupNode(const std::string &name, Node *parent,
            GroupNode **currentParentNode)
      : Node(name, parent), currentParentNode(currentParentNode) {}

  void addNode(std::unique_ptr<Node> node) {
    subnodes.push_back(std::move(node));
  }

  void action() override { *currentParentNode = this; }

  Node *getSelectedNode() {
    if (selection == -1) {
      return nullptr;
    }

    return subnodes[selection].get();
  }

  void render() override {
    std::string str;
    int indexAtSelection = -1;

    for (int i = -1; i < subnodes.size(); i++) {
      std::string name = "..";

      if (i != -1) {
        auto &subnodePtr = subnodes[i];
        name = subnodePtr->name;
      }

      if (selection == i) {
        indexAtSelection = str.size();
        str += "[" + name + "]";
      } else {
        str += name;
      }

      str += " ";
    }
    str += ">";

    std::string outputStr = str.substr(indexAtSelection, 16);

    std::cout << "\n\n";
    std::cout << outputStr << "\n\n";
    LCD::clearAndSet(outputStr, "");
  }

  void right() override {
    selection += 1;
    if (selection >= subnodes.size()) {
      selection = -1;
    }
  }

  void left() override {
    selection -= 1;
    if (selection < -1) {
      selection = subnodes.size() - 1;
    }
  }
};
