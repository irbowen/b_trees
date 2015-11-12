#include "seq_tree.h"

void Sequential_Tree::insert(int key, int value) {
  //Wory about splitting here too
  Node_key temp;
  //If this returns true, than that means the root has been split
  if (root->add_key_value_pair(key, value, temp)) {
    std::cout << "We're gonna have to add " << key << " as the only key in the new root node\n";
  }
}

void Sequential_Tree::print_all() {
  /*
  for (auto& child : children) {
    std::cout << child->key << " " << child->value << "\n";
  }
  */
}
