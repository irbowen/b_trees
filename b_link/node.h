#ifndef NODE_H
#define NODE_H

#include <vector>
#include <list>
#include <tuple>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <stack>
#include <mutex>

#include "helper.h"

/*  Maxiumum number of data slots in leaf nodes */
const size_t DATA_SLOTS = 8;

/*  Maximum number of children in inner nodes */
const size_t FAN_OUT = 8;

/* Forward declaration to make the compiler happy */
class Node;

class Node_key {
public:
  int key;
  Node* node = nullptr;
  bool is_leaf;
};


/*  Empty superclass of nodes.
    Basically so we can have data structs of Node* */
class Node {
public:
  static int counter;
  int get_counter();
  int high_key;
  bool is_leaf;
  Node* link_ptr;

  virtual bool can_split() = 0;
  virtual bool add_key_value_pair(int, int, Node_key&, Node_key&, int) = 0;
  virtual int search(int) = 0;
  virtual std::string print_r(int) = 0;
  virtual void print_link(int) = 0;
  virtual void check_keys() = 0;
};

#endif
