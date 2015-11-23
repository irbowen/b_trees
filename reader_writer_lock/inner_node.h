#ifndef INNER_NODE_H
#define INNER_NODE_H

#include <cassert>
#include <mutex>
#include <thread>
#include <shared_mutex>
#include <chrono>

#include "node.h"
#include "reader_writer_lock.h"

/* Class for the inner nodes in the tree */
class Inner_Node : public Node {
  /* */
  void add_to_child(std::list<Node*>::iterator, int, int);
  Reader_Writer_Lock node_lock;
  std::shared_timed_mutex node_mutex;
  std::mutex m;
public:
  Inner_Node();
  std::list<int> keys;
  std::list<Node*> values;

  int unique_id;
  void create_first_node(int, int);
  bool add_key_value_pair(int, int, Node_key&);
  void add_vector_keys(std::list<int>);
  void add_vector_nodes(std::list<Node*>);
  void add_key(int);
  void print_keys();
  void print_r(int);
  bool can_split();
};

#endif
