#pragma once
#include <memory>
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
