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
  if (keys.size() + 1 >= FAN_OUT) {
    return true;
  }
  return false;
}

/*  Return true if this nodes splits, false if not  */
bool Inner_Node::add_key_value_pair(int key, int value, Node_key& node_key) {
  /*  So, we want to get an exlusive lock at first */ 
  //  node_mutex.lock();
  unique_lock<shared_timed_mutex> e_lock(node_mutex);
  //  We don't want to lock this yet
  shared_lock<shared_timed_mutex> s_lock(node_mutex, defer_lock);
  
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
      child_can_split = (*this_value)->can_split();
      if (!child_can_split) {
        safe_cout("Okay, i'm trying to downgrad this lock, since it can't split");
        m.lock();
        e_lock.unlock();
        s_lock.lock();
        m.unlock();
      }
     /*  So, is it expensive to grab the write lock first, and then maybe downgrade?
          Well, yes - but its correct.  We at first need to find the node to insert on, and 
          we know nothing about the node.  Once we KNOW FOR SURE THAT ITS SAFE, we can downgrade
          to a shared lock. */
      temp_value = keys.size();
      add_to_child(this_value, key, value);
      new_value = keys.size();
      inserted = true;
      break;
    }
  }
  /*  Probably need the same code from above ^^^, down here */
  //  auto max_key = std::max_element(begin(keys), end(keys));
  if (!inserted) { // && key > *max_key) {
    child_can_split = (*this_value)->can_split();
    if (!child_can_split) {
      safe_cout("Downgrading to shared_lock in END case");
      m.lock();
      e_lock.unlock();
      s_lock.lock();
      m.unlock();
    }
    temp_value_end = keys.size();
    add_to_child(this_value, key, value);
    new_value_end = keys.size();
    inserted = true;
  }
  if (!child_can_split) {
    ostringstream temp_oss;
    temp_oss << "Child cannot split.  Keys size is: " << keys.size() << "\n";
    for (auto& k : keys) {
      temp_oss << k << " ";
    }
    temp_oss << "\n";
    safe_cout(temp_oss.str());
  }
  if (!child_can_split) {
    ostringstream temp_oss1;
    temp_oss1 << "Unlocking shared and returning\n" 
        << "!child_can_split before after key size: " << temp_value << " " << new_value << endl
        << "!child_can_split (end case) before after key size: " << temp_value_end << " " << new_value_end << endl;
    safe_cout(temp_oss1.str());
    assert(s_lock.owns_lock());
    return false;
  }
  if (child_can_split && keys.size() < FAN_OUT ) {
    safe_cout("Unlocking exlusive and returning\n");
    assert(e_lock.owns_lock());
    return false;
  }
  if (child_can_split) {
    assert(e_lock.owns_lock());
    //std::cout << "--IN:AKVP::Splitting inner node\n";
    auto mid_point = FAN_OUT / 2;
    auto keys_it = begin(keys);
    auto value_it = begin(values);
    for (size_t i = 0; i < mid_point; i++) {
      keys_it++;
      value_it++;
    }
    /*  So then this IS our key */
    auto new_key = *keys_it;
    keys_it++; value_it++;
    
    std::list<int> right_keys(keys_it, end(keys));
    std::list<Node*> right_values(value_it, end(values));
    //right_values.push_front(new Leaf_Node());
    //std::cout << "Old array: ";
    //for (size_t i = 0; i < keys.size(); i++) { std::cout << keys.at(i) << " ";}
    //std::cout << std::endl;
    keys.resize(mid_point+1);
    values.resize(mid_point+1);
//    values.push_back(new Leaf_Node());
    //std::cout << "New array: ";
    //for (size_t i = 0; i < keys.size(); i++) { std::cout << keys.at(i) << " ";}
    //std::cout << "--IN:AKVP::New key: " << new_key << std::endl;
    Inner_Node* right_inner_node = new Inner_Node();
    right_inner_node->add_vector_keys(right_keys);
    right_inner_node->add_vector_nodes(right_values);
    node_key.node = right_inner_node;
    node_key.key = new_key;
    //std::cout << "--IN:AKVP::Returning true for inner node add_key_value_pair" << std::endl;
    node_mutex.unlock();
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
    //std::cout << "----Returned true from add_key_value_pair" << std::endl;
   // std::cout << "----Adding: <key: " << temp.key << ", value: " << temp.node << "> to an inner node\n";
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
//      cout << "dist: " << dist << endl;
  //    auto new_dist = distance(value_it, begin(values));
 //     cout << "new dist: " << new_dist << endl;
      index++;
      
   //   auto index_dist = distance(index, begin(values));
  //    cout << "index distance: " << index_dist << endl;
//      assert(index_dist == dist);
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
  oss << "} ";
  return oss.str();
}

