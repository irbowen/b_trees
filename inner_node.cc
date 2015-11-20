#include "inner_node.h"
#include "leaf_node.h"

using namespace std;

/*  Just assinged the node's unique_id... Nothing else to do */
Inner_Node::Inner_Node() {
  unique_id = Node::get_counter();
}

/*  Return true if this nodes splits, false if not  */
bool Inner_Node::add_key_value_pair(int key, int value, Node_key& node_key) {
  /*  If this is the very first Inner_Node in the tree, we have to do some setup */
  if (keys.size() == 0) {
    create_first_node(key, value);
    return false;
  }
  
  bool inserted = false;
  auto values_post = begin(values);
  auto this_key = begin(keys);
  for (; this_key != end(keys); this_key++, values_post++) {
    if (key <= *this_key) {
        add_to_child(values_post, key, value);
        //std::cout << "Node: " << i << "key, key compare: " << key << ", " << keys.at(i) << " "<< std::endl;
        inserted = true;
        // std::cout << " Inserted key into this innner node\n" << std::endl;
        break;
      }
    }


    auto max_key = std::max_element(begin(keys), end(keys));
  //  std::cout << "Max key: " << *max_key << std::endl;
    if (!inserted && key > *max_key) {
      //auto min_index = std::min(keys.size(), FAN_OUT);
     // std::cout << "FANOUT, key size: " << FAN_OUT << " " << keys.size() << std::endl;
   //      std::cout << "Node: " << keys.size() << "key, key compare: " << key << ", " << keys.at(keys.size()-1) << " "<< std::endl;
      add_to_child(values_post, key, value);
//      std::cout << "Had to add it at the end of the inner node\n" << std::endl;
      //std::cout << "inserting at end\n" <<std::endl;
      inserted = true;
    }
    if (!inserted)
      cout << "ABORT: SOMETHING HAS GONE VERY WRONG\n";
    // cout << "--Sent request to child for key " << key << "\n";
  //}
  if (keys.size() >= FAN_OUT) {
    //std::cout << "--IN:AKVP::Splitting inner node\n";
    auto mid_point = FAN_OUT / 2;
    auto keys_it = begin(keys);
    auto value_it = begin(values);
    for (size_t i = 0; i < mid_point; i++) {
      keys_it++;
      value_it++;
    }
    /*  So then this is our key */
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
    values.push_back(new Leaf_Node());
    //std::cout << "New array: ";
    //for (size_t i = 0; i < keys.size(); i++) { std::cout << keys.at(i) << " ";}
    //std::cout << "--IN:AKVP::New key: " << new_key << std::endl;
    Inner_Node* right_inner_node = new Inner_Node();
    right_inner_node->add_vector_keys(right_keys);
    right_inner_node->add_vector_nodes(right_values);
    node_key.node = right_inner_node;
    node_key.key = new_key;
    //std::cout << "--IN:AKVP::Returning true for inner node add_key_value_pair" << std::endl;
    return true;
  }
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
      auto new_dist = distance(value_it, begin(values));
 //     cout << "new dist: " << new_dist << endl;
      index++;
      
      auto index_dist = distance(index, begin(values));
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

void Inner_Node::print_r(int depth) {
  string padding(depth * 2, ' ');
  ostringstream oss;
  oss << padding << "Inner(" << unique_id << "): (";
  for (auto& k : keys) {
    oss << k << ", ";
  }
  oss << ")->{\n";
  cout << oss.str();
  for (auto& v : values) {
    v->print_r(depth+1);
    cout << endl;
  }
  cout << padding  << "} ";
}

