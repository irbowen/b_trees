#ifndef SEQ_TREE_H
#define SEQ_TREE_H

#include <vector>
#include <algorithm>
#include <iostream>

#include "node.h"

class Sequential_Tree {
public:
  Node* root;
  Sequential_Tree() {
    root = new Inner_Node();
  }
  void insert(int, int);
  void split();
  void print_all();
};

#endif
