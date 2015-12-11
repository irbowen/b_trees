#ifndef LEAF_NODE_H
#define LEAF_NODE_H

#include "node.h"
#include <stack>
#include <signal.h>

/* Class for the leaf nodes of the tree */
class Leaf_Node : public Node {
  /*  The actual data to be stored.  The first element is the key,
      the second is the value.  Right now the value is an int -
      I was thinking disk block number - but this could be anything */  
  std::list<std::tuple<int, int>> elements;
  
  /*  The reader writer lock object for this object */
  std::mutex m;
  //std::unique_lock<std::mutex> leaf_lock(m, std::defer_lock);
public:
  int unique_id;
  int high_key;
  bool is_leaf;
  /*  Creates a new leaf node */
  Leaf_Node();
  
  /*  Inherited from Node.  Add the (int,int) pair, Node_key
    is used if the node is split */
  bool add_key_value_pair(int, int, Node_key&, Node_key&, int);
  
  int search(int);
  
  /*  Add the argument vector to the nodes internal elements vecotr */
  void add_vector(std::list<std::tuple<int, int>>);
  
  /*  Print the elements, with spacing based on the int arg */
  std::string print_r(int);
  
  void print_link(int);
  void unlock_leaf();
  void check_keys();
  
  /* Print all the keys stored in elements vector */
  std::string print_keys();
  bool can_split();
};

#endif
