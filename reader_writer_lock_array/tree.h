#ifndef TREE_H
#define TREE_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <mutex>

#include "leaf_node.h"
#include "inner_node.h"

class Tree {
public:
  Inner_Node* root;
  std::mutex m;
  std::shared_timed_mutex node_mutex;
  Tree() {
    root = new Inner_Node();
    root->create_first_node(1, 1);
    std::cout << "Created root node\n";
  }
  int get_value(int);
  void insert(int, int);
  void print_all();
};

#endif
