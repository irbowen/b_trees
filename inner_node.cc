#include "inner_node.h"
#include "leaf_node.h"

using namespace std;

bool Inner_Node::add_key_value_pair(int key, int value, Node_key& node_key) {
 // print_keys();
  // If nothing is in this inner_node
  if (keys.size() == 0) {
    create_first_node(key, value);
    // We can return false right away, because we know this node cannot split
    return false;
  }
  //if (keys.size() < FAN_OUT) {
    // cout << "--Inserting into inner node\n";
  // Because we always want to insert
  if (true) {
    bool inserted = false;
    for (size_t i = 0; i < keys.size(); i++) {
      if (key <= keys.at(i)) {
        add_to_child(i, key, value);
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
      add_to_child(keys.size(), key, value);
//      std::cout << "Had to add it at the end of the inner node\n" << std::endl;
      //std::cout << "inserting at end\n" <<std::endl;
      inserted = true;
    }
    if (!inserted)
      cout << "ABORT: SOMETHING HAS GONE VERY WRONG\n";
    // cout << "--Sent request to child for key " << key << "\n";
  }
  if (keys.size() >= FAN_OUT) {
    //std::cout << "--IN:AKVP::Splitting inner node\n";
    auto mid_point = FAN_OUT / 2;
    //std::cout << "--IN:AKVP::Midpoint, size: " << mid_point << ", " << keys.size() << "\n";
    //std::cout << std::endl;
    auto new_key = keys.at(mid_point);
    std::vector<int> right_keys(begin(keys) + mid_point, end(keys));
    std::vector<Node*> right_values(begin(values) + mid_point, end(values));
    //std::cout << "Old array: ";
    //for (size_t i = 0; i < keys.size(); i++) { std::cout << keys.at(i) << " ";}
    //std::cout << std::endl;
    keys.resize(mid_point + 1);
    values.resize(mid_point + 1);
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
  //Change this
  node_key.node = nullptr;
  //std::cout << "--IN:AKVP::Done with add_key_value_pair " << key << ", set size: " << keys.size() << std::endl;
  return false;
}

/*  Add (key, value) to the child node at index
    If the child node splits, add the key and new node to this inner node */
void Inner_Node::add_to_child(int index, int key, int value) {
  Node_key temp;
  if (values.at(index)->add_key_value_pair(key, value, temp)) {
    //std::cout << "----Returned true from add_key_value_pair" << std::endl;
   // std::cout << "----Adding: <key: " << temp.key << ", value: " << temp.node << "> to an inner node\n";
    /*  Search for the key.  If we can't find it, add to the end */ 
    auto it = lower_bound(begin(keys), end(keys), key);
    if (it == end(keys)) {
      keys.push_back(temp.key);
      values.push_back(temp.node);
    }
    /*  If we can find it, that is where we have to insert */
    else {
      auto index = it - keys.begin();
      keys.insert(it, temp.key);
      values.insert(values.begin() + index, temp.node);
    }
  }
}

/*  Creates two leaf nodes, or each side of the first key.
    Only needs to be called once */
void Inner_Node::create_first_node(int key, int value) {
  cout << "NOTE: INTIAL SETUP\n";
  //cout << "--Added key " << key << " to inner node\n";
  //To get things started, we need a leaf node on each side of the first key
  keys.push_back(key);
  values.push_back(new Leaf_Node());
  values.push_back(new Leaf_Node());
  //And then insert the new (key, value) on the left of the key
  Node_key temp;
  values.at(0)->add_key_value_pair(key, value, temp);
}

void Inner_Node::add_key(int i) {
  keys.push_back(i);
}

void Inner_Node::add_vector_keys(std::vector<int> v) {
  for (auto& elem : v) {
    keys.push_back(elem);
  }
}

void Inner_Node::add_vector_nodes(std::vector<Node*> v) {
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
  string padding(depth, ' ');
  ostringstream oss;
  oss << endl;
  oss << padding << "Inner(";
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

