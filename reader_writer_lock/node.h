#ifndef NODE_H
#define NODE_H

#include <vector>
#include <list>
#include <tuple>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#include "helper.h"

/*  Maxiumum number of data slots in leaf nodes */
const size_t DATA_SLOTS = 16;

/*  Maximum number of children in inner nodes */
const size_t FAN_OUT = 16;

/* Forward declaration to make the compiler happy */
class Node;

class Node_key {
public:
  int key;
  Node* node = nullptr;
};

/*  Emptry superclass of nodes.
    Basically so we can have data structs of Node* */
class Node {
public:
  static int counter;
  int get_counter();
  virtual bool can_split() = 0;
  virtual bool add_key_value_pair(int, int, Node_key&) = 0;
  virtual void reset() = 0;
  virtual bool is_inner() = 0;
  /*  In Inner_Nodes, this will call thsi function for each node below it
      For Leaf Nodes, this will print out all the keys */
  virtual std::string  print_r(int) = 0;
};

#endif
