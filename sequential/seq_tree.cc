#include "seq_tree.h"

using namespace std;

Sequential_Tree::Sequential_Tree() {
  root = new Inner_Node();
  root->create_first_node(1, 1);
}

int Sequential_Tree::get_value(int key) {
  return root->get_value(key);
}

void Sequential_Tree::insert(int key, int value) {
  bool inserted = false;
  auto this_value = begin(root->values);
  auto this_key = begin(root->keys);
  for (; this_key != end(root->keys); this_key++, this_value++) {
    if (key <= *this_key) {
      assert(!inserted);
      assert(*this_value);
      assert((*this_value) != nullptr);
      root->add_to_child(this_value, key, value);
      inserted = true;
      break;
    }
  }
  if (!inserted) {
    assert(this_key == end(root->keys));
    if (this_value != end(root->values)) {
      assert(root->values.size() > root->keys.size());
      root->add_to_child(this_value, key, value);
      inserted = true;
    }
  }
  assert(inserted);
  if (root->keys.size() >= FAN_OUT) {
    auto mid_point = FAN_OUT / 2;
    auto keys_it = begin(root->keys);
    auto value_it = begin(root->values);
    for (size_t i = 0; i < mid_point; i++) {
      keys_it++;
      value_it++;
    }
    auto new_key = *keys_it;
    keys_it++;
    value_it++;
    /*  We make the new data structs for the new ndoes */
    std::list<int> right_keys(keys_it, end(root->keys));
    std::list<Node*> right_values(value_it, end(root->values));
    /*  Change this node to be the new left node */
    root->keys.resize(mid_point+1);
    root->values.resize(mid_point+1);
    root->values.push_back(new Leaf_Node());

    Inner_Node* right_inner_node = new Inner_Node();
    right_inner_node->add_vector_keys(right_keys);
    right_inner_node->add_vector_nodes(right_values);
    /*  Setup to return to the caller */
    Inner_Node* new_root = new Inner_Node();
    list<Node*> new_children;
    new_children.push_back(root);
    new_children.push_back(right_inner_node);
    new_root->add_key(new_key);
    new_root->add_vector_nodes(new_children);
    root = new_root;
    return;
  }
}

void Sequential_Tree::print_all() {
  ostringstream oss;
  oss << "\n==PRINT ALL==\n";
  oss << root->print_r(1);
  oss << "\n" << endl;
  cout << oss.str();
}
