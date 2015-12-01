#ifndef TREE_H
#define TREE_H

#include <vector>
#include <algorithm>
#include <iostream>
#include <mutex>

#include "inner_node.h"
#include "reader_writer_lock.h"

class Tree {
public:
  Inner_Node* root;
  std::mutex m;
  std::shared_timed_mutex node_mutex;
  Tree() {
    root = new Inner_Node();
    root->create_first_node(1, 1);
  }
  void insert(int, int);
  void split();
  void print_all();
};

#endif
