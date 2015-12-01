#include "inner_node.h"
#include "leaf_node.h"

using namespace std;

/*  Not using dynamic locking, so uneeded */
/*  Just assinged the node's unique_id... Nothing else to do */
Inner_Node::Inner_Node() {
  unique_id = Node::get_counter();
}

/*  How thread safe does this need to be? */
bool Inner_Node::can_split() {
  unique_lock<mutex> lock(m);
  if (keys.size() + 1 >= FAN_OUT) {
    return true;
  }
  return false;
}

/*  Return true if this nodes splits, false if not  */
bool Inner_Node::add_key_value_pair(int key, int value, Node_key& node_key) {
  /*  So, we want to get an exlusive lock at first */ 
  //  node_mutex.lock();

  /*  These locks:
      - Protect from any exceptions
      - Make sure the mutex is unlocked after the function call
      - Let us check if the lock owns the mutex (handy for asserts!)
      So we will use them instead of the raw mutex's 
      We grab the e_lock right away, and just declare */
  unique_lock<shared_timed_mutex> e_lock(node_mutex, defer_lock);
  shared_lock<shared_timed_mutex> s_lock(node_mutex);
  
  bool inserted = false, child_can_split = false;
  /*  What is this values_post */ 
  auto this_value = begin(values);
  auto this_key = begin(keys);
  size_t temp_value{0}, temp_value_end{0}, new_value{0}, new_value_end{0};
  /*  If this key is less than or eqal to the current, 
      we want to insert into its left(down) child */
  for (; this_key != end(keys); this_key++, this_value++) {
    /*  We want to see if this is the key */
    if (key <= *this_key) {
      assert(!inserted);
      if (m.try_lock()) {
        child_can_split = (*this_value)->can_split();
        if (child_can_split) {
          safe_cout("Okay, i'm trying to upgrade this lock, since it can split");
          s_lock.unlock();
          e_lock.lock();
        }
        else {
          safe_cout("Maintaining shared lock");
        }
        temp_value = keys.size();
        //ostringstream oss1; oss1 << "ChildCanSplit: " << child_can_split << "\nBefore: ";
        //for (auto& k : keys) {oss1 << k << " ";}oss1<<"\n";
        add_to_child(this_value, key, value);
        //oss1 << "After: ";
        //for (auto& k : keys) {oss1 << k << " ";}oss1<<"\nKey:" << key << "\n";
        //safe_cout(oss1.str());
        new_value = keys.size();
        inserted = true;
        m.unlock();
        break;
      }
      else {
        s_lock.unlock();
        safe_cout("I couldn't get the lock, so I'm calling the function again\n");
        return add_key_value_pair(key, value, node_key);
      }
    }
  }
  /*  Probably need the same code from above ^^^, down here */
  //  auto max_key = std::max_element(begin(keys), end(keys));
  if (!inserted) { // && key > *max_key) {
    assert(s_lock.owns_lock());
    assert(this_key == end(keys));
    if (m.try_lock()) {
      child_can_split = (*this_value)->can_split();
      if (child_can_split) {
        safe_cout("Okay, i'm trying to upgrade this lock, since it can split");
        s_lock.unlock();
        e_lock.lock();
      }
      else {
        safe_cout("Maintinng shared");
      }
      temp_value_end = keys.size();
      add_to_child(this_value, key, value);
      new_value_end = keys.size();
      inserted = true;
      m.unlock();
    }
    else {
      s_lock.unlock();
      safe_cout("I couldn't get the lock, so I'm calling the function again\n");
      return add_key_value_pair(key, value, node_key);
    }
  }
  
  assert(inserted);
  if (!child_can_split) {
    /*  We want to make sure the values have not changed, if the child could not have split */
    assert(temp_value == new_value);
    assert(temp_value_end == new_value_end);
    assert(s_lock.owns_lock());
    return false;
  }
  if (child_can_split && keys.size() < FAN_OUT) {
    safe_cout("Unlocking exlusive and returning\n");
    assert(e_lock.owns_lock());
    return false;
  }
  if (child_can_split && keys.size() >= FAN_OUT) {
    assert(e_lock.owns_lock());
    /*  We need to find the midpoint, keys, etc. */
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
    /*  We make the new data structs for the new ndoes */
    std::list<int> right_keys(keys_it, end(keys));
    std::list<Node*> right_values(value_it, end(values));
    /*  Change this node to be the new left node */
    keys.resize(mid_point+1);
    values.resize(mid_point+1);
    /*  Add everything over into the new inner node */
    Inner_Node* right_inner_node = new Inner_Node();
    right_inner_node->add_vector_keys(right_keys);
    right_inner_node->add_vector_nodes(right_values);
    /*  Setup to return to the caller */
    node_key.node = right_inner_node;
    node_key.key = new_key;
    return true;
  }
  safe_cout("THIS SHOULD NEVER HAPPEN\nSOMETHING IS WRONG WITH MY LOGIC AND LOCKING\nHALP");
  exit(1);
  return false;
}

/*  Add (key, value) to the child node at index
    If the child node splits, add the key and new node to this inner node */
void Inner_Node::add_to_child(list<Node*>::iterator index, int key, int value) {
  Node_key temp;
  if ((*index)->add_key_value_pair(key, value, temp)) {
    /*  Search for the key.  If we can't find it, add to the end */ 
    auto keys_it = upper_bound(begin(keys), end(keys), key);
    if (keys_it == end(keys)) {
      keys.push_back(temp.key);
      values.push_back(temp.node);
    }
    /*  If we can find it, that is where we have to insert */
    else {
      auto dist = distance(keys_it, begin(keys));
      auto value_it = begin(values);
      for (auto i = 0; i < dist; i++) {value_it++;} 
      keys.insert(keys_it, temp.key);
      index++;
      values.insert(index, temp.node);
    }
  }
}

/*  Creates two leaf nodes, or each side of the first key.
    Only needs to be called once */
void Inner_Node::create_first_node(int key, int value) {
 // cout << "NOTE: INTIAL SETUP\n";
  //cout << "--Added key " << key << " to inner node\n";
  //To get things started, we need a leaf node on each side of the first key
  keys.push_back(key);
  values.push_back(new Leaf_Node());
  values.push_back(new Leaf_Node());
  //And then insert the new (key, value) on the left of the key
  Node_key temp;
  (*begin(values))->add_key_value_pair(key, value, temp);
}

void Inner_Node::add_key(int i) {
  keys.push_back(i);
}

void Inner_Node::add_vector_keys(std::list<int> v) {
  for (auto& elem : v) {
    keys.push_back(elem);
  }
}

void Inner_Node::add_vector_nodes(std::list<Node*> v) {
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

