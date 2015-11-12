#include "node.h"

using namespace std;

bool Inner_Node::add_key_value_pair(int key, int value, Node_key& node_key) {
  print_keys();
  // If nothing is in this inner_node
  if (keys.size() == 0) {
    create_first_node(key, value);
  }
  if (keys.size() < FAN_OUT) {
    cout << "--Inserting into inner node\n";
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
    }
    if (!inserted)
      cout << "ABORT: SOMETHING HAS GONE VERY WRONG\n";
    cout << "--Sent request to child for key " << key << "\n";
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

void Inner_Node::print_keys() {
  std::cout << "--INNER node keys: ";
  for (auto& k : keys) {
    std::cout << k << " ";
  }
  std::cout << std::endl;
}

bool Leaf_Node::add_key_value_pair(int key, int value, Node_key& node_key) {
  std::cout << "--LEAF node keys: ";
  for (auto& e : elements) {
    std::cout << std::get<0>(e) << " ";
  }
  std::cout << std::endl;
  // If we don't have to split the node
  if (elements.size() < DATA_SLOTS) {
    //std::cout << "Adding to a leaf node without splitting\n";
    elements.push_back(std::make_tuple(key, value));
    std::sort(begin(elements), end(elements), [](auto& a, auto& b) {
      return std::get<0>(a) < std::get<0>(b);
    });
    std::cout << "--Added key " << key << " to leaf node\n";
    return false;
  }
  else {
    std::cout << "Adding to a leaf node WITH splitting\n";
    auto mid_point = DATA_SLOTS / 2;
    auto new_key = elements.at(mid_point);
    std::cout << "The new key is: " << std::get<0>(new_key);
    std::vector<std::tuple<int, int>> left(begin(elements), begin(elements) + mid_point);
    std::vector<std::tuple<int, int>> right(begin(elements) + mid_point, end(elements));
   
    std::cout << "\nOld array:";
    for (auto& a : elements) { std::cout << std::get<0>(a) << " " ; }
    std::cout << "\nLeft array:";
    for(auto& a : left) { std::cout << std::get<0>(a) << " "; }
    std::cout << "\nRight array:";
    for(auto& a : right) { std::cout << std::get<0>(a) << " "; }

    elements.resize(mid_point);
    node_key.key = std::get<0>(new_key);
    node_key.node = new Leaf_Node();
    std::cout << "\nCreated a new leaf node\n" << std::endl;
    reinterpret_cast<Leaf_Node*>(node_key.node)->add_vector(right);
    return true;
  }


}

void Leaf_Node::add_vector(std::vector<std::tuple<int, int>> v) {
  for (auto& i : v) {
    elements.push_back(i);
  }
}


