#ifndef SEQ_TREE_H
#define SEQ_TREE_H

#include <vector>
#include <algorithm>
#include <iostream>

class Sequential_Tree {

  class Node {
  public:
    int key;
    int value;  
    Node(int key_, int value_) : key(key_), value(value_) {}
  };

  std::vector<Node*> children;

public:
  Sequential_Tree();
  void insert(int, int);
  void print_all();
};

#endif
