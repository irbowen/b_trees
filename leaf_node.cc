#include "leaf_node.h"

using namespace std;

Leaf_Node::Leaf_Node() {
  elements.reserve(DATA_SLOTS);
  left_sibling = nullptr;
  right_sibling = nullptr;
  unique_id = Node::get_counter();
}

bool Leaf_Node::add_key_value_pair(int key, int value, Node_key& node_key) {
  /*- Debugging output -*/
  /*  std::cout << "--LEAF node keys: ";
      print_keys();
      std::cout << "Elements size: " << elements.size() << " " << std::endl; */
  /*  We always want to insert into this node.  We can worry about splits lates */
  elements.push_back(std::make_tuple(key, value));
  std::sort(begin(elements), end(elements), [](auto& a, auto& b) {
    return std::get<0>(a) < std::get<0>(b);
  });
  if (elements.size() >= DATA_SLOTS) {
    //std::cout << "--SPLITTING LEAF NODE\n";
    auto mid_point = DATA_SLOTS / 2;
    auto new_key = elements.at(mid_point);
    //std::cout << "----New Key: " << std::get<0>(new_key);
    std::vector<std::tuple<int, int>> right(begin(elements) + mid_point, end(elements));
    /* 
    std::cout << "\n----Old array:";
    for (auto& a : elements) { std::cout << std::get<0>(a) << " " ; }
    std::cout << "\n----Right array:";
    for(auto& a : right) { std::cout << std::get<0>(a) << " "; }
    */
    elements.resize(mid_point);
    //std::cout << "ELEMENTS NEW SIZE: " << elements.size() << std::endl;

    node_key.key = std::get<0>(new_key);
    node_key.node = new Leaf_Node();
    /*  Shuffling pointers around */
    Leaf_Node* temp = right_sibling;
    right_sibling = reinterpret_cast<Leaf_Node*>(node_key.node);
    right_sibling->left_sibling = this;
    right_sibling->right_sibling = temp;
    right_sibling->add_vector(right);
    return true;
  }
  return false;
}

/*  Copies all the elements of the vector into the node's internal storage
    TODO: make this a move, &&, or something else fancy/fast */
void Leaf_Node::add_vector(std::vector<std::tuple<int, int>> v) {
  for (auto& i : v) {
    elements.push_back(i);
  }
}

/*  Prints all the keys of the elements stored in this leaf node */
void Leaf_Node::print_r(int depth) {
  string padding(depth * 2, ' ');
  cout << padding << "Leaf(" << unique_id << "): [";
  print_keys();
  cout << "]";
}

void Leaf_Node::print_keys() {
  for (auto& e : elements) {
    cout << get<0>(e) << ", ";
  }
}
