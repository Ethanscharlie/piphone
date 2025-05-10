#pragma once
#include <functional>
#include <string>

#include "Node.hpp"

class FunctionNode : public Node {
  std::function<void()> function;

public:
  FunctionNode(const std::string &name, Node *parent,
               std::function<void()> function)
      : Node(name, parent), function(function) {}

  void action() override { function(); }
};
