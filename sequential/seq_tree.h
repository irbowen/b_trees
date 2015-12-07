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
  Sequential_Tree();
  void insert(int, int);
  void print_all();
  int get_value(int);
};

#endif
