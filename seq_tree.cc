#include "seq_tree.h"

Sequential_Tree::Sequential_Tree() {
  std::cout << "Created a sequential tree\n";
}


void Sequential_Tree::insert(int key, int value) {
  
  if (children.size() > 10) {
    //need to do split
    std::cout << "Need to split node, over size\n";
  }
  else {
    children.push_back(new Node(key, value));
    std::sort(begin(children), end(children), [](Node* a, Node* b) {
      return a->key > b->key;
    });
  }

}

void Sequential_Tree::print_all() {
  for (auto& child : children) {
    std::cout << child->key << " " << child->value << "\n";
  }
}
