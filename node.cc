#include "node.h"

bool Inner_Node::add_key_value_pair(int key, int value, Node_key*& node_key) {
  // If nothing is in this inner_node
  if (keys.size() == 0) {
    keys.push_back(key);
    values.push_back(new Leaf_Node());
    Node_key* temp = nullptr;
    values.at(0)->add_key_value_pair(key, value, temp);
  }
  else if (keys.size() < FAN_OUT) {
    std::cout << "We just need to push the data into the leaf nodes\n";
    bool inserted = false;
    for (int i = 1; i < FAN_OUT; i++) {
      if (key < keys.at(i)) {
        Node_key* temp = nullptr;
        if (values.at(i-1)->add_key_value_pair(key, value, temp)) {
          keys.push_back(temp->key);
          values.push_back(temp->node);
        }
        inserted = true;
        break;
      }
    }
    if (!inserted) {
      Node_key* temp = nullptr;
      if (values.at(FAN_OUT)->add_key_value_pair(key, value, temp)) {
        keys.push_back(temp->key);
        values.push_back(temp->node);
      }
    }
  }
  if (keys.size() >= FAN_OUT) {
    std::cout << "We have to split the inner node\n";
    return true;
  }
  return false;
}

bool Leaf_Node::add_key_value_pair(int key, int value, Node_key* &node_key) {
  // If we don't have to split the node
  if (elements.size() < DATA_SLOTS) {
    elements.push_back(std::make_tuple(key, value));
    std::sort(begin(elements), end(elements), [](auto& a, auto& b) {
      return std::get<0>(a) < std::get<1>(b);
    });
    return false;
  }
  else {
    auto mid_point = DATA_SLOTS / 2;
    auto new_key = elements.at(mid_point);
    std::cout << "\nThe new key is: " << std::get<0>(new_key);
    std::vector<std::tuple<int, int>> left(begin(elements), begin(elements) + mid_point);
    std::vector<std::tuple<int, int>> right(begin(elements) + mid_point, end(elements));
    std::cout << "\nLeft array:";
    for(auto& a : left) {
      std::cout << std::get<0>(a) << " ";
    }
    std::cout << "\nRight array:";
    for(auto& a : right) {
      std::cout << std::get<0>(a) << " ";
    }
    Leaf_Node* left_node = new Leaf_Node(left);
    Leaf_Node* right_node = new Leaf_Node(right);
    return true;
  }
}