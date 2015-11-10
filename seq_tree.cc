#include "seq_tree.h"

void Sequential_Tree::insert(int key, int value) {
  //Wory about splitting here too
  Node_key temp;
  root->add_key_value_pair(key, value, temp);
}

void Sequential_Tree::print_all() {
  /*
  for (auto& child : children) {
    std::cout << child->key << " " << child->value << "\n";
  }
  */
}
