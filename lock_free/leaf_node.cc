#include "leaf_node.h"

using namespace std;

/*  Create a new Leaf_Node.  At this point, sibling pointers don't need to 
    point to anything */
Leaf_Node::Leaf_Node() {
}

bool Leaf_Node::can_split() {
  if (elements.size() + 1 >= DATA_SLOTS) {
    return true;
  }
  return false;
}

/*  Step 1: Make a copy of the keys
    Step 2: Try to do the insert
    Step 3: Do the atomic compare
    Step 4: Maybe? Continue        */
bool Leaf_Node::add_key_value_pair(int key, int value, Node_key& node_key) {
  /*  We always want to insert into this node.  We can worry about splits lates */
  atomic<list<tuple<int, int>>*> elements_copy = elements;
  elements_copy->push_back(make_tuple(key, value));
  /*  Since this is a list, we can't just use std::sort() */
  elements_copy->sort([](auto& a, auto& b) {
    return get<0>(a) < get<0>(b);
  });
  /*  But what if we need to split the node? */
  if (elements_copy->size() >= DATA_SLOTS) {
    /*  Find the midpoint element in the list.... Linear search */
    auto mid_point = elements_copy.size() / 2;
    auto it = begin(elements_copy);
    for (size_t i = 0; i < mid_point; i++) {
      it++;
    }
    /*  So then this is our key */
    auto new_key = *it;
    /*  And we want our key to have its descendants on the left. */
    it++;
    list<tuple<int, int>> right(it, end(elements_copy));
    /*  Resize, since we're giving half of our list to the new node */
    elements_copy->resize(mid_point + 1);
    /*  Set the node_key to pass back up */
    node_key.key = get<0>(new_key);
    node_key.node = new Leaf_Node();
    reinterpret_cast<Leaf_Node*>(node_key.node)->add_vector(right);
    /*  This could probably be released earlier, but for correctness, for now... */
    return true;
  }
  return false;
}

/*  Copies all the elements of the vector into the node's internal storage
    TODO: make this a move, &&, or something else fancy/fast 
    NOTE: NOT THREAD SAFE */
void Leaf_Node::add_vector(list<tuple<int, int>> v) {
  for (auto& i : v) {
    elements.push_back(i);
  }
}

/*  Prints all the keys of the elements stored in this leaf node for debugging 
    NOTE: NOT THREAD SAFE*/
string Leaf_Node::print_r(int depth) {
  ostringstream oss;
  string padding(depth * 2, ' ');
  //cout << padding << "Leaf(" << unique_id << "): [";
  oss  << padding << "Leaf(): [";
  oss << print_keys();
  oss << "]";
  return oss.str();
}

/*  Print all the keys in stored in this leaf node 
    NOTE: NOT THREAD SAFE*/
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
