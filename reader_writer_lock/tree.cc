#include "tree.h"

using namespace std;

void Tree::insert(int key, int value) {
  unique_lock<shared_timed_mutex> e_lock(node_mutex);
  ostringstream oss;
  oss << "INSERTiNG KEY: " << key << "\n";
  safe_cout(oss.str());
  //Wory about splitting here too
  Node_key temp;
  //If this returns true, than that means the root has been split
  //, defer_lock);
 // shared_lock<shared_timed_mutex> s_lock(node_mutex);
  /* if (m.try_lock()) {
    if (root->can_split()) {
      s_lock.unlock();
      e_lock.lock();
    } */
    if (root->add_key_value_pair(key, value, temp)) {
      safe_cout("Root is splitting\n");
      assert(e_lock.owns_lock());
    //  assert(!s_lock.owns_lock());
      Inner_Node* new_root = new Inner_Node();
      list<Node*> new_children;
      new_children.push_back(root);
      new_children.push_back(temp.node);
      new_root->add_key(temp.key);
      new_root->add_vector_nodes(new_children);
      root = new_root;
    }
 /*   m.unlock();
  }
  else {
    s_lock.unlock();
    safe_cout("I couldn't get the lock, so I'm calling the function again\n");
    return insert(key, value);
  } */
}

void Tree::print_all() {
  ostringstream oss;
  oss << "\n==PRINT ALL==\n";
  oss << root->print_r(1);
  oss << "\n" << endl;
  safe_cout(oss.str());
}

