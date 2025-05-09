#pragma once
#include <string>

#include "Node.hpp"

class LabelNode : public Node {
public:
  LabelNode(const std::string &name, Node *parent) : Node(name, parent) {}
  void action() override {}
};
