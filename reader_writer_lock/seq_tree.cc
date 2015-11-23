#include "seq_tree.h"

using namespace std;

void Sequential_Tree::insert(int key, int value) {
  ostringstream oss;
  oss << "INSERTiNG KEY: " << key << "\n";
  safe_cout(oss.str());
  //Wory about splitting here too
  Node_key temp;
  //If this returns true, than that means the root has been split
  if (root->add_key_value_pair(key, value, temp)) {
   // cout << "--RNS ROOT NODE SPLITTING\n";
  //  cout << "--RNS  NEW KEY: " << key << "\n";
    Inner_Node* new_root = new Inner_Node();
    list<Node*> new_children;
    new_children.push_back(root);
    new_children.push_back(temp.node);
    new_root->add_key(temp.key);
    new_root->add_vector_nodes(new_children);
    //reinterpret_cast<Inner_Node*>(root)->print_keys();
   // reinterpret_cast<Inner_Node*>(temp.node)->print_keys();
    root = new_root;
   // root->print_keys();
  }
  print_all();
}

void Sequential_Tree::print_all() {
  ostringstream oss;
  oss << "\n==PRINT ALL==\n";
  oss << root->print_r(1);
  oss << "\n" << endl;
  safe_cout(oss.str());
}

