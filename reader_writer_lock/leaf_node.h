#ifndef LEAF_NODE_H
#define LEAF_NODE_H

#include "node.h"
#include "reader_writer_lock.h"

/* Class for the leaf nodes of the tree */
class Leaf_Node : public Node {
  /*  The actual data to be stored.  The first element is the key,
      the second is the value.  Right now the value is an int -
      I was thinking disk block number - but this could be anything */  
  std::list<std::tuple<int, int>> elements;
  /*  The reader writer lock object for this object */
  std::mutex m;
public:
  /*  Left and right siblings so that we can avoid 
      going up to the parent to do range queries */
  Leaf_Node* left_sibling;
  Leaf_Node* right_sibling;

  //int unique_id;
  /*  Creates a new leaf node */
  Leaf_Node();
  /*  Inheirteed from Node.  Add the (int,int) pair, Node_key
    is used if the node is split */
  bool add_key_value_pair(int, int, Node_key&);
  /*  Add the argument vector to the nodes internal elements vecotr */
  void add_vector(std::list<std::tuple<int, int>>);
  /*  Print the elements, with spacing based on the int arg */
  std::string print_r(int);
  /* Print all the keys stored in elements vector */
  std::string  print_keys();
  bool can_split();
  void reset();
  bool is_inner();
};

#endif
