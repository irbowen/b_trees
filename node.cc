#include "node.h"

bool Inner_Node::add_key_value_pair(int key, int value, Node_key& node_key) {
  std::cout << "Keys in this INNER node: ";
  for (auto& k : keys) {
    std::cout << k << " ";
  }
  std::cout << std::endl;
  // If nothing is in this inner_node
  if (keys.size() == 0) {
    std::cout << "Inital setup\n";
    keys.push_back(key);
    values.push_back(new Leaf_Node());
    values.push_back(new Leaf_Node());
    Node_key temp;
    values.at(0)->add_key_value_pair(key, value, temp);
  }
  else if (keys.size() < FAN_OUT) {
    bool inserted = false;
    for (size_t i = 0; i < keys.size(); i++) {
      if (key <= keys.at(i)) {
        add_to_child(i, key, value);
        inserted = true;
        break;
      }
    }
    auto max_z = std::max_element(begin(keys), end(keys));
    //std::cout << "Max key in this nodeis: " << *max_z << std::endl;
    if (!inserted && key > *max_z) {
      auto min_index = std::min(keys.size(), FAN_OUT);
      add_to_child(min_index, key, value);
    }
  }
  if (keys.size() >= FAN_OUT) {
    std::cout << "We have to split the inner node\n";
    node_key.node = new Inner_Node();
    return true;
  }
  std::cout << std::endl;
  return false;
}

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

bool Leaf_Node::add_key_value_pair(int key, int value, Node_key& node_key) {
  std::cout << "Keys in this LEAF node: ";
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
    for (auto& a : elements) {
      std::cout << std::get<0>(a) << " " ;
    }
    std::cout << "\nLeft array:";
    for(auto& a : left) {
      std::cout << std::get<0>(a) << " ";
    }
    std::cout << "\nRight array:";
    for(auto& a : right) {
      std::cout << std::get<0>(a) << " ";
    }

    node_key.key = std::get<0>(new_key);
    node_key.node = new Leaf_Node();
    std::cout << "\nCreated a new leaf node" << std::endl << std::endl;
    reinterpret_cast<Leaf_Node*>(node_key.node)->add_vector(right);
    std::cout << std::endl;
    return true;
  }


}

void Leaf_Node::add_vector(std::vector<std::tuple<int, int>> v) {
  for (auto& i : v) {
    elements.push_back(i);
  }
}


