#include "leaf_node.h"

using namespace std;

Leaf_Node::Leaf_Node() {
  elements.reserve(DATA_SLOTS + 2);
}

bool Leaf_Node::can_split() {
  if (elements.size() + 1 >= DATA_SLOTS) {
    return true;
  }
  return false;
}

int Leaf_Node::get_value(int key) {
  shared_lock<shared_timed_mutex> lock(m);
  auto results = find_if(elements.begin(), elements.end(), [&](auto& a) {
    return (get<0>(a) == key);
  });
  if (results != end(elements)) {
    return get<1>(*results);
  }
  return -1;
}

bool Leaf_Node::add_key_value_pair(int key, int value, Node_key& node_key) {
  unique_lock<shared_timed_mutex> lock(m);
  elements.push_back(make_tuple(key, value));
  sort(begin(elements), end(elements), [](auto& a, auto& b) {
    return get<0>(a) < get<0>(b);
  });
  if (elements.size() >= DATA_SLOTS) {
    auto mid_point = elements.size() / 2;
    auto it = begin(elements);
    for (size_t i = 0; i < mid_point; i++) {
      it++;
    }
    auto new_key = *it;
    it++;
    vector<tuple<int, int>> right(it, end(elements));
    elements.resize(mid_point + 1);
    node_key.key = get<0>(new_key);
    node_key.node = new Leaf_Node();
    reinterpret_cast<Leaf_Node*>(node_key.node)->add_vector(right);
    return true;
  }
  return false;
}

void Leaf_Node::add_vector(vector<tuple<int, int>> v) {
  for (auto& i : v) {
    elements.push_back(i);
  }
}

string Leaf_Node::print_r(int depth) {
  ostringstream oss;
  string padding(depth * 2, ' ');
  oss  << padding << "Leaf(): [";
  oss << print_keys();
  oss << "]";
  return oss.str();
}

string Leaf_Node::print_keys() {
  ostringstream oss; 
  for (auto& e : elements) {
    oss << get<0>(e) << ", ";
  }
  return oss.str();
}

bool Leaf_Node::is_inner() {
  return false;
}
