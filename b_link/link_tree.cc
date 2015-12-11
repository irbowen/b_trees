#include "link_tree.h"
#include <stack>

using namespace std;

void Link_Tree::insert(int key, int value, int thread_num) {
  ostringstream oss;
  oss << "INSERTiNG KEY: " << key << "\n";
  safe_cout(oss.str(), thread_num);
  //Wory about splitting here too
  Node_key temp;
	Node_key last_locked;	
	bool release = true;
  //If this returns true, than that means the root has been split
  if (root->add_key_value_pair(key, value, temp, last_locked, thread_num)) {
		//Should already have lock on root at this point
		oss.str("Root splitting\n");
	  safe_cout(oss.str(), thread_num);
		
  	if(last_locked.is_leaf){
  		
  		oss.str("");
			oss << "Locking root node(" << root->unique_id << ") in link_tree" << endl;
		  safe_cout(oss.str(), thread_num);

			root->lock_inner();  	
  	}
  	else{
  		Inner_Node* locked_node = (Inner_Node*)last_locked.node;
  		if(locked_node->unique_id != root->unique_id){

				oss.str("");
				oss << "Locking root node(" << root->unique_id << ") in link_tree" << endl;
			  safe_cout(oss.str(), thread_num);
			  
				root->lock_inner();
				release = false;
			}
  	}
  	if(last_locked.is_leaf){
  		Leaf_Node* leafPtr = (Leaf_Node*)last_locked.node;
	  	
	  	oss.str("");
	  	oss << "Unocking leaf(" << leafPtr->unique_id << ") in link_tree" << endl;
		  safe_cout(oss.str(), thread_num);

  		leafPtr->unlock_leaf();
  	}
  	else if(release){
  		Inner_Node* innerPtr = (Inner_Node*)last_locked.node;

			oss.str("");
			oss << "Unocking node(" << innerPtr->unique_id << ") in link_tree" << endl;
		  safe_cout(oss.str(), thread_num);

			innerPtr->unlock_inner();
  	}
  	
    Inner_Node* new_root = new Inner_Node();
    list<Node*> new_children;
    new_children.push_back(root);
    new_children.push_back(temp.node);
    new_root->add_key(temp.key);
    new_root->add_vector_nodes(new_children);
    Inner_Node* old_root = root;
    root = new_root;
    root->high_key = temp.node->high_key;
    first_split = false;
    
  	oss.str("");
  	oss << "Unocking root node(" << old_root->unique_id << ") in link_tree" << endl;
	  safe_cout(oss.str(), thread_num);
    
    old_root->unlock_inner();
  }
  //root->check_keys();
  //print_all();
}

int Link_Tree::search(int key){
	return root->search(key);
}

void Link_Tree::print_all() {
  ostringstream oss;
  oss << "\n==PRINT ALL==\n";
  oss << root->print_r(1);
  oss << "\n" << endl;
  safe_cout(oss.str(), 0);
}

