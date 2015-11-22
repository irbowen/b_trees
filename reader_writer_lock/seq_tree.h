#ifndef SEQ_TREE_H
#define SEQ_TREE_H

#include <vector>
#include <algorithm>
#include <iostream>

#include "inner_node.h"
#include "dynamic_locker.h"

class Sequential_Tree {
  Dyanmic_Locker lock_table;
public:
  Inner_Node* root;
  Sequential_Tree() {
    root = new Inner_Node();
  }
  void insert(int, int);
  void split();
  void print_all();
};

#endif
