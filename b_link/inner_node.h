#ifndef INNER_NODE_H
#define INNER_NODE_H

#include <cassert>
#include <mutex>
#include <thread>
#include <shared_mutex>
#include <chrono>
#include <stack>
#include <signal.h>

#include "node.h"

/* Class for the inner nodes in the tree */
class Inner_Node : public Node {
   
  bool add_to_child(/*std::list<Node*>::iterator,*/Node*, int, int, Node_key&, Node_key&, int);
  std::shared_timed_mutex node_mutex;
  std::mutex m;
public:
  Inner_Node();
  std::list<int> keys;
  std::list<Node*> values;

	int high_key;
  int unique_id;
  bool is_leaf;
  void check_keys();
  void unlock_inner();
  void lock_inner();
  void create_first_node(int, int);
  bool add_key_value_pair(int, int, Node_key&, Node_key&, int);
  int search(int);
  void add_vector_keys(std::list<int>);
  void add_vector_nodes(std::list<Node*>);
  void add_key(int);
  void print_keys();
  std::string print_r(int);
  void print_link(int);
  bool can_split();
};

#endif
