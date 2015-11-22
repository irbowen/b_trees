#include "leaf_node.h"

using namespace std;

/*  Create a new Leaf_Node.  At this point, sibling pointers don't need to 
    point to anything */
Leaf_Node::Leaf_Node() {
  left_sibling = nullptr;
  right_sibling = nullptr;
  //unique_id = Node::get_counter();
}

bool Leaf_Node::can_split() {
  if (elements.size() + 1 >= DATA_SLOTS) {
    return true;
  }
  return false;
}

bool Leaf_Node::add_key_value_pair(int key, int value, Node_key& node_key) {
  /* We're clearly here to do a write */
  node_lock.write_lock();
  /*  We always want to insert into this node.  We can worry about splits lates */
  elements.push_back(make_tuple(key, value));
  /*  Since this is a list, we can't just use std::sort() */
  elements.sort([](auto& a, auto& b) {
    return get<0>(a) < get<0>(b);
  });
  /*  But what if we need to split the node? */
  if (elements.size() >= DATA_SLOTS) {
    /*  Find the midpoint element in the list.... Linear search */
    auto mid_point = elements.size() / 2;
    auto it = begin(elements);
    for (size_t i = 0; i < mid_point; i++) {
      it++;
    }
    /*  So then this is our key */
    auto new_key = *it;
    /*  And we want our key to have its descendants on the left. */
    it++;
    list<tuple<int, int>> right(it, end(elements));
    /*  Resize, since we're giving half of our list to the new node */
    elements.resize(mid_point + 1);
    /*  Set the node_key to pass back up */
    node_key.key = get<0>(new_key);
    node_key.node = new Leaf_Node();
    /*  Shuffling pointers around */
    Leaf_Node* temp = right_sibling;
    right_sibling = reinterpret_cast<Leaf_Node*>(node_key.node);
    right_sibling->left_sibling = this;
    right_sibling->right_sibling = temp;
    right_sibling->add_vector(right);
    /*  This could probably be released earlier, but for correctness, for now... */
    node_lock.write_unlock();
    return true;
  }
  node_lock.write_unlock();
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
void Leaf_Node::print_r(int depth) {
  string padding(depth * 2, ' ');
  //cout << padding << "Leaf(" << unique_id << "): [";
  cout << padding << "Leaf(): [";
  print_keys();
  cout << "]";
}

/*  Print all the keys in stored in this leaf node 
    NOTE: NOT THREAD SAFE*/
void Leaf_Node::print_keys() {
  for (auto& e : elements) {
    cout << get<0>(e) << ", ";
  }
}
