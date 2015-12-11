#ifndef LINK_TREE_H
#define LINK_TREE_H

#include <vector>
#include <algorithm>
#include <iostream>

#include "inner_node.h"
#include "leaf_node.h"


class Link_Tree {
public:
  Inner_Node* root;
  bool first_split;
  Link_Tree() {
    root = new Inner_Node();
    root->create_first_node(1000, 1000);
    first_split = true;
  }
  void insert(int, int, int);
  int search(int);
  void split();
  void print_all();
};

#endif
