#include "tree.h"

using namespace std;

int Tree::get_value(int key) {
  shared_lock<shared_timed_mutex> s_lock(node_mutex);
  return root->get_value(key);
}

void Tree::insert(int key, int value) {
  unique_lock<shared_timed_mutex> e_lock(node_mutex, defer_lock);
  shared_lock<shared_timed_mutex> s_lock(node_mutex);
  bool inserted = false, child_can_split = false;
  auto this_value = begin(root->values);
  auto this_key = begin(root->keys);
  size_t temp_value{0}, temp_value_end{0}, new_value{0}, new_value_end{0};
  assert(*this_value);
  assert(*this_key);
  for (; this_key != end(root->keys); this_key++, this_value++) {
    /*  We want to see if this is the key */
    if (key <= *this_key) {
      assert(!inserted);
      if (m.try_lock()) {
        assert(*this_value);
        child_can_split = (*this_value)->can_split();
        if (child_can_split) {
          s_lock.unlock();
          e_lock.lock();
        }
        temp_value = root->keys.size();
        root->add_to_child(this_value, key, value);
        new_value = root->keys.size();
        inserted = true;
        m.unlock();
        break;
      }
      else {
        assert(s_lock.owns_lock());
        s_lock.unlock();
        return insert(key, value);
      }
    }
  }
  if (!inserted) {
    assert(s_lock.owns_lock());
    assert(this_key == end(root->keys));
    if (m.try_lock()) {
      assert(this_value != end(root->values));
      assert(*this_value);
      if (this_value != end(root->values)) {
        assert(root->values.size() > root->keys.size());
        child_can_split = (*this_value)->can_split();
        if (child_can_split) {
          s_lock.unlock();
          e_lock.lock();
        }
        temp_value_end = root->keys.size();
        root->add_to_child(this_value, key, value);
        new_value_end = root->keys.size();
        inserted = true;
      }
      m.unlock();
    }
    else {
      assert(s_lock.owns_lock());
      s_lock.unlock();
      return insert(key, value);
    }
  }
  
  assert(inserted);
  if (!child_can_split) {
    /*  We want to make sure the values have not changed, if the child could not have split */
    assert(temp_value == new_value);
    assert(temp_value_end == new_value_end);
    assert(s_lock.owns_lock());
    return;
  }
  if (child_can_split && root->keys.size() < FAN_OUT) {
    //safe_cout("Unlocking exlusive and returning\n");
    assert(e_lock.owns_lock());
    return;
  }
  if (child_can_split && root->keys.size() >= FAN_OUT) {
    assert((temp_value_end != new_value_end) || (temp_value != new_value));
    assert(e_lock.owns_lock());
    /*  We need to find the midpoint, keys, etc. */
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
  safe_cout("THIS SHOULD NEVER HAPPEN\nSOMETHING IS WRONG WITH MY LOGIC AND LOCKING\nHALP");
  exit(1);
}

void Tree::print_all() {
  ostringstream oss;
  oss << "\n==PRINT ALL==\n";
  oss << root->print_r(1);
  oss << "\n" << endl;
  safe_cout(oss.str());
}
