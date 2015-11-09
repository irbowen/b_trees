#ifndef NODE_H
#define NODE_H

#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>

/*  Maxiumum number of data slots in leaf nodes */
const int DATA_SLOTS = 10;

/*  Maximum number of children in inner nodes */
const int FAN_OUT = 10;

class Node;

class Node_key {
public:
  int key;
  Node* node;
};

/*  Emptry superclass of nodes.
    Basically so we can have data structs of Node* */
class Node {
public:
  virtual bool add_key_value_pair(int, int, Node_key*&) = 0;
};


/* Class for the inner nodes in the tree */
class Inner_Node : public Node {
  /*  
      */
  std::vector<int> keys;
  std::vector<Node*> values;
public:
  Inner_Node() {
    keys.reserve(FAN_OUT);
    values.reserve(FAN_OUT + 1);
  }
  bool add_key_value_pair(int, int, Node_key*&);
};

/* Class for the leaf nodes of the tree */
class Leaf_Node : public Node {
  std::vector<std::tuple<int, int>> elements;
public:
  Leaf_Node() {
    elements.reserve(DATA_SLOTS);
  }
  Leaf_Node(std::vector<std::tuple<int, int>> v) : elements(v) {}
  bool add_key_value_pair(int, int, Node_key*&);
};

#endif
