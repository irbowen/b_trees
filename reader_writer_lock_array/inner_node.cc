#include "inner_node.h"
#include "leaf_node.h"

using namespace std;

int Inner_Node::get_value(int key) {
  shared_lock<shared_timed_mutex> s_lock(node_mutex);
  assert(values.size() > keys.size());
  auto this_value = begin(values);
  auto this_key = begin(keys);
  assert(*this_value);
  assert(*this_key);
  for (; this_key != end(keys); this_key++, this_value++) {
    if (key <= *this_key) {
      return (*this_value)->get_value(key);
    }
  }
  assert(values.size() > keys.size());
  assert(*this_value);
  return (*this_value)->get_value(key);
}

Inner_Node::Inner_Node() {
  keys.reserve(FAN_OUT + 2);
  values.reserve(FAN_OUT + 3);
}

bool Inner_Node::is_inner() {
  return true;
}

bool Inner_Node::can_split() {
  unique_lock<mutex> lock(m);
  if (keys.size() + 1 >= FAN_OUT) {
    return true;
  }
  return false;
}

bool Inner_Node::add_key_value_pair(int key, int value, Node_key& node_key) {
  unique_lock<shared_timed_mutex> e_lock(node_mutex, defer_lock);
  shared_lock<shared_timed_mutex> s_lock(node_mutex);
  assert(values.size() > keys.size());
  bool inserted = false, child_can_split = false;
  auto this_value = begin(values);
  auto this_key = begin(keys);
  size_t temp_value{0}, temp_value_end{0}, new_value{0}, new_value_end{0};
  assert(*this_value);
  assert(*this_key);
  for (; this_key != end(keys); this_key++, this_value++) {
    if (key <= *this_key) {
      assert(!inserted);
      if (m.try_lock()) {
        assert((*this_value) != nullptr);
        child_can_split = (*this_value)->can_split();
        if (child_can_split) {
          s_lock.unlock();
          e_lock.lock();
        }
        temp_value = keys.size();
        assert(values.size() > keys.size());
        add_to_child(this_value, key, value);
        assert(values.size() > keys.size());
        new_value = keys.size();
        inserted = true;
        m.unlock();
        break;
      }
      else {
        assert(s_lock.owns_lock());
        s_lock.unlock();
        return add_key_value_pair(key, value, node_key);
      }
    }
  }
  assert(values.size() > keys.size());
  assert(this_value != end(values));
  assert(*this_value);
  if (!inserted) {
    assert(s_lock.owns_lock());
    assert(this_key == end(keys));
    if (m.try_lock()) {
      if (this_value != end(values)) {
        assert(values.size() > keys.size());
        child_can_split = (*this_value)->can_split();
        if (child_can_split) {
          s_lock.unlock();
          e_lock.lock();
        }
        temp_value_end = keys.size();
        assert(values.size() > keys.size());
        add_to_child(this_value, key, value);
        assert(values.size() > keys.size());
        new_value_end = keys.size();
        inserted = true;
      }
      m.unlock();
    }
    else {
      assert(s_lock.owns_lock());
      s_lock.unlock();
      return add_key_value_pair(key, value, node_key);
    }
  }
  assert(inserted);
  if (!child_can_split) {
    assert(temp_value == new_value);
    assert(temp_value_end == new_value_end);
    assert(s_lock.owns_lock());
    return false;
  }
  if (child_can_split && keys.size() < FAN_OUT) {
    assert(e_lock.owns_lock());
    return false;
  }
  if (child_can_split && keys.size() >= FAN_OUT) {
    assert((temp_value_end != new_value_end) || (temp_value != new_value));
    assert(e_lock.owns_lock());
    auto mid_point = FAN_OUT / 2;
    auto keys_it = begin(keys);
    auto value_it = begin(values);
    for (size_t i = 0; i < mid_point; i++) {
      keys_it++;
      value_it++;
    }
    auto new_key = *keys_it;
    keys_it++;
    value_it++;
    vector<int> right_keys(keys_it, end(keys));
    vector<Node*> right_values(value_it, end(values));
    keys.resize(mid_point+1);
    values.resize(mid_point+1);
    values.push_back(new Leaf_Node());
    Inner_Node* right_inner_node = new Inner_Node();
    right_inner_node->add_vector_keys(right_keys);
    right_inner_node->add_vector_nodes(right_values);
    node_key.node = right_inner_node;
    node_key.key = new_key;
    assert(values.size() > keys.size());
    assert(right_inner_node->values.size() > right_inner_node->keys.size());
    return true;
  }
  assert(false);
}

void Inner_Node::add_to_child(vector<Node*>::iterator index, int key, int value) {
  Node_key temp;
  if ((*index)->add_key_value_pair(key, value, temp)) {
    auto keys_it = lower_bound(begin(keys), end(keys), temp.key);
    if (keys_it == end(keys)) {
      keys.push_back(temp.key);
      values.push_back(temp.node);
    }
    else {
      auto dist = distance(keys_it, begin(keys));
      auto value_it = begin(values);
      for (auto i = 0; i < dist; i++) {value_it++;} 
      index++;
      keys.insert(keys_it, temp.key);
      values.insert(index, temp.node);
    }
  }
}

void Inner_Node::create_first_node(int key, int value) {
  unique_lock<shared_timed_mutex> lock(node_mutex);  
  keys.push_back(key);
  values.push_back(new Leaf_Node());
  values.push_back(new Leaf_Node());
  Node_key temp;
  (*begin(values))->add_key_value_pair(key, value, temp);
}

void Inner_Node::add_key(int i) {
  keys.push_back(i);
}

void Inner_Node::add_vector_keys(vector<int> v) {
  for (auto& elem : v) {
    keys.push_back(elem);
  }
}

void Inner_Node::add_vector_nodes(vector<Node*> v) {
  for (auto& elem : v) {
    values.push_back(elem);
  }
}

void Inner_Node::print_keys() {
  cout << "--INNER node keys: ";
  for (auto& k : keys) {
    cout << k << " ";
  }
  cout << endl;
}

string Inner_Node::print_r(int depth) {
  string padding(depth * 2, ' ');
  ostringstream oss;
  oss << padding << "Inner(" << unique_id << "): (";
  for (auto& k : keys) {
    oss << k << ", ";
  }
  oss << ")->{\n";
  for (auto& v : values) {
    oss << v->print_r(depth+1) << "\n";
  }
  oss << padding << "} ";
  return oss.str();
}

