#include "leaf_node.h"

using namespace std;

bool Leaf_Node::add_key_value_pair(int key, int value, Node_key& node_key) {
  std::cout << "--LEAF node keys: ";
  print_r();
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
    std::cout << "--SPLITTING LEAF NODE\n";
    auto mid_point = DATA_SLOTS / 2;
    auto new_key = elements.at(mid_point);
    std::cout << "----New Key: " << std::get<0>(new_key);
    std::vector<std::tuple<int, int>> left(begin(elements), begin(elements) + mid_point);
    std::vector<std::tuple<int, int>> right(begin(elements) + mid_point, end(elements));
   
    std::cout << "\n----Old array:";
    for (auto& a : elements) { std::cout << std::get<0>(a) << " " ; }
    std::cout << "\n----Left array:";
    for(auto& a : left) { std::cout << std::get<0>(a) << " "; }
    std::cout << "\n----Right array:";
    for(auto& a : right) { std::cout << std::get<0>(a) << " "; }

    elements.resize(mid_point);
    node_key.key = std::get<0>(new_key);
    node_key.node = new Leaf_Node();
    std::cout << "\n----Created a new leaf node" << std::endl;
    reinterpret_cast<Leaf_Node*>(node_key.node)->add_vector(right);
    return true;
  }
}

void Leaf_Node::add_vector(std::vector<std::tuple<int, int>> v) {
  for (auto& i : v) {
    elements.push_back(i);
  }
}

void Leaf_Node::print_r() {
  for (auto& e : elements) {
    cout << get<0>(e) << ", ";
  }
}
