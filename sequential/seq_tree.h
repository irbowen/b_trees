#ifndef SEQ_TREE_H
#define SEQ_TREE_H

#include <vector>
#include <algorithm>
#include <iostream>

#include "inner_node.h"
#include "leaf_node.h"

class Sequential_Tree {
public:
  Inner_Node* root;
  Sequential_Tree() {
    root = new Inner_Node();
    root->create_first_node(1,1);
  }
  void insert(int, int);
  void split();
  void print_all();
  int get_value(int);
};

#endif
