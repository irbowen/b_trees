#include "inner_node.h"
#include "leaf_node.h"

using namespace std;

bool Inner_Node::add_key_value_pair(int key, int value, Node_key& node_key) {
  print_keys();
  // If nothing is in this inner_node
  if (keys.size() == 0) {
    create_first_node(key, value);
  }
  if (keys.size() < FAN_OUT) {
    // cout << "--Inserting into inner node\n";
    bool inserted = false;
    for (size_t i = 0; i < keys.size(); i++) {
      if (key <= keys.at(i)) {
        add_to_child(i, key, value);
        inserted = true;
        break;
      }
    }
    auto max_key = std::max_element(begin(keys), end(keys));
    if (!inserted && key > *max_key) {
      auto min_index = std::min(keys.size() - 1, FAN_OUT);
      add_to_child(min_index, key, value);
      inserted = true;
    }
    if (!inserted)
      cout << "ABORT: SOMETHING HAS GONE VERY WRONG\n";
    // cout << "--Sent request to child for key " << key << "\n";
  }
  if (keys.size() >= FAN_OUT) {
    std::cout << "--Splitting inner node\n";
    auto mid_point = FAN_OUT / 2;
    std::cout << "Midpoint, size: " << mid_point << ", " << keys.size() << "\n";
    std::cout << std::endl;
    auto new_key = keys.at(mid_point);
    std::vector<int> right_keys(begin(keys), end(keys) + mid_point);
    std::vector<Node*> right_values(begin(values) + mid_point, end(values));
    std::cout << "Old array: ";
    for (size_t i = 0; i < keys.size(); i++) { std::cout << keys.at(i) << " ";}
    std::cout << std::endl;
    keys.resize(mid_point + 1);
    values.resize(mid_point + 1);
    std::cout << "New array: ";
    for (size_t i = 0; i < keys.size(); i++) { std::cout << keys.at(i) << " ";}
    std::cout << std::endl;
    std::cout << "New key: " << new_key << std::endl;
    node_key.node = new Inner_Node();
    node_key.key = new_key;
    return true;
  }
  //Change this
  node_key.node = nullptr;
  std::cout << std::endl;
  return false;
}

/*  Add (key, value) to the child node at index
    If the child node splits, add the key and new node to this inner node */
void Inner_Node::add_to_child(int index, int key, int value) {
  Node_key temp;
  if (values.at(index)->add_key_value_pair(key, value, temp)) {
    std::cout << "We're adding: <key: " << temp.key << ", value: " << temp.node << "> to an inner node\n";
    auto it = lower_bound(begin(keys), end(keys), key);
    auto index = it - keys.begin();
    keys.insert(it, temp.key);
    values.insert(values.begin() + index, temp.node);
  }
}

/*  Creates two leaf nodes, or each side of the first key.
    Only needs to be called once */
void Inner_Node::create_first_node(int key, int value) {
  cout << "NOTE: INTIAL SETUP\n";
  cout << "--Added key " << key << " to inner node\n";
  //To get things started, we need a leaf node on each side of the first key
  keys.push_back(key);
  values.push_back(new Leaf_Node());
  values.push_back(new Leaf_Node());
  //And then insert the new (key, value) on the left of the key
  Node_key temp;
  values.at(0)->add_key_value_pair(key, value, temp);
}

void Inner_Node::add_key(int i) {
  keys.push_back(i);
}

void Inner_Node::add_vector_keys(std::vector<int> v) {
  for (auto& elem : v) {
    keys.push_back(elem);
  }
}

void Inner_Node::add_vector_nodes(std::vector<Node*> v) {
  for (auto& elem : v) {
    values.push_back(elem);
  }
}

void Inner_Node::print_keys() {
  std::cout << "--INNER node keys: ";
  for (auto& k : keys) {
    std::cout << k << " ";
  }
  std::cout << std::endl;
}

void Inner_Node::print_r() {
  for (auto& v : values) {
    v->print_r();
  }
}

