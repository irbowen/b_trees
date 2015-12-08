#ifndef TREE_H
#define TREE_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <mutex>

#include "leaf_node.h"
#include "inner_node.h"

class Tree {
  Inner_Node* root;
  std::mutex m;
  std::shared_timed_mutex node_mutex;
public:
  Tree() {
    root = new Inner_Node();
    root->create_first_node(1, 1);
  }
  int get_value(int);
  void insert(int, int);
  void print_all();
};

#endif