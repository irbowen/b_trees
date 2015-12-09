#ifndef NODE_H
#define NODE_H

#include <vector>
#include <list>
#include <tuple>
#include <algorithm>
#include <iostream>
#include <sstream>

/*  Maxiumum number of data slots in leaf nodes */
const size_t DATA_SLOTS = 256;

/*  Maximum number of children in inner nodes */
const size_t FAN_OUT = 256;

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
  virtual bool add_key_value_pair(int, int, Node_key&)    = 0;
  virtual int  get_value(int)                             = 0;
  virtual std::string  print_r(int)                       = 0;
};

#endif
