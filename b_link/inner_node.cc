#include "inner_node.h"
#include "leaf_node.h"
#include <stack>
using namespace std;

/*  Not using dynamic locking, so uneeded */
/*  Just assinged the node's unique_id... Nothing else to do */
Inner_Node::Inner_Node() {
  unique_id = Node::get_counter();
  link_ptr = nullptr;
  high_key = -1;
  is_leaf = false;
}

/*  How thread safe does this need to be? */
bool Inner_Node::can_split() {
  if (keys.size() >= FAN_OUT) {
  	//std::cout << "INNER NODE IS FULL\n";
    return true;
  }
  return false;
}

/*  Return true if this nodes splits, false if not  */
bool Inner_Node::add_key_value_pair(int key, int value, Node_key& node_key, Node_key& last_locked, int thread_num){

	Node_key child_split_key;
  bool inserted = false;
	bool need_to_split = false;
	ostringstream oss;

  /*  What is this values_post */ 
  auto this_value = begin(values);
  auto this_key = begin(keys);

  /*  If this key is less than or eqal to the current, we want to insert into its left(down) child */
  
  oss.str("");
  oss << "Searching node(" << unique_id << ")\n";
  safe_cout(oss.str(), thread_num);
  
  for (; this_key != end(keys); this_key++, this_value++) {
    if (key <= *this_key) {    	
      need_to_split = add_to_child(*this_value, key, value, last_locked, child_split_key, thread_num);
      inserted = true;
      break;
    }
  }

  /*  If didn't find in loop, add to rightmost child */
  if (!inserted) {
  	if(link_ptr != nullptr){
  		need_to_split = link_ptr->add_key_value_pair(key, value, node_key, last_locked, thread_num);
  	}
  	else{
		  safe_cout("In this here end case\n", thread_num);  
		  need_to_split = add_to_child(*this_value, key, value, last_locked, child_split_key, thread_num);
    }
  }
  
  if(!need_to_split){
  	return false;
  }
  
  //IF WE GET HERE, THE NODE SHOULD BE LOCKED
	Inner_Node* inner = (Inner_Node*)last_locked.node;  
	
	oss.str("");
	oss << "Holding lock on node(" << inner->unique_id << ") in add_key" << endl;	
	safe_cout(oss.str(), thread_num);
	  
  if (inner->keys.size() == FAN_OUT && need_to_split) {
    auto mid_point = FAN_OUT / 2;
    auto keys_it = begin(inner->keys);
    auto value_it = begin(inner->values);
    
    for (size_t i = 0; i < mid_point - 1; i++) {
      keys_it++;
      value_it++;
    }

    /*  So then this is our key */
    auto new_key = *keys_it;
    
    oss.str("");
    oss << "NEW INNER KEY = " << *keys_it << endl;
  	safe_cout(oss.str(), thread_num);
	  
    keys_it++; value_it++;
    
    std::list<int> right_keys(keys_it, end(inner->keys));
    std::list<Node*> right_values(value_it, end(inner->values));
    inner->keys.resize(mid_point);
    inner->values.resize(mid_point);
    Inner_Node* right_inner_node = new Inner_Node();
    
    bool found_flag = false;
    if(child_split_key.key <= new_key){
    	//add to left list
    	oss.str("");
    	oss << "INSERTING  " << child_split_key.key << " INTO LEFT CHILD\n";
    	safe_cout(oss.str(), thread_num);
			
			value_it = inner->values.begin();
    	for(keys_it = inner->keys.begin(); keys_it != inner->keys.end(); keys_it++, value_it++){
    		if(*keys_it > child_split_key.key){
    			//std::cout << "INSERTING " << child_split_key.key << " IN FRONT OF " << *keys_it << endl;
    			inner->keys.insert(keys_it, child_split_key.key);
    			inner->values.insert(++value_it, child_split_key.node);
    			found_flag = true;
    			break;
    		}
    	}
    	if(!found_flag){
    			//std::cout << "INSERTING " << child_split_key.key << " AT END OF KEYS" << endl;
    			inner->keys.push_back(child_split_key.key);
    			inner->values.push_back(child_split_key.node);    	
    	}
    }
    else{
    	//add to right list
    	oss.str("");
    	oss << "INSERTING " << child_split_key.key << " INTO RIGHT CHILD\n";
    	safe_cout(oss.str(), thread_num);
			
			auto right_values_it = right_values.begin();
    	for(auto right_keys_it = right_keys.begin(); right_keys_it != right_keys.end(); right_keys_it++, right_values_it++){
    		if(*right_keys_it > child_split_key.key){
    			//std::cout << "INSERTING " << child_split_key.key << " IN FRONT OF " << *right_keys_it << endl;
    			right_keys.insert(right_keys_it, child_split_key.key);
    			right_values.insert(++right_values_it, child_split_key.node);
    			found_flag = true;
    			break;
    		}
  		}
	  	if(!found_flag){
  				//std::cout << "INSERTING " << child_split_key.key << " AT END OF RIGHT KEYS" << endl;
	  			right_keys.push_back(child_split_key.key);
	  			right_values.push_back(child_split_key.node);    	
    	}
    }
    
    //update link pointers
   	right_inner_node->link_ptr = inner->link_ptr;
   	inner->link_ptr = right_inner_node;
   	
   	//update high keys
   	right_inner_node->high_key = inner->high_key;
   	inner->high_key = keys.back();
   	
    right_inner_node->add_vector_keys(right_keys);
    right_inner_node->add_vector_nodes(right_values);
    node_key.node = right_inner_node;
    node_key.node->high_key = right_inner_node->high_key;
    node_key.key = new_key;
    last_locked.node = inner;
    return true;
  }

  cout  << "THIS SHOULD NEVER HAPPEN\nSOMETHING IS WRONG WITH MY LOGIC AND LOCKING\nHALP";
  
  raise(SIGSEGV);
  
  exit(1);
  return false;
}

/*  Add (key, value) to the child node at index
    If the child node splits, add the key and new node to this inner node */
bool Inner_Node::add_to_child(Node* index, int key, int value, Node_key& last_locked, Node_key& child_split_key, int thread_num) {
  Node_key temp;
 	Inner_Node* inner = this;
 	ostringstream oss;

	//TODO:SEGFAULTING HERE
  if (index->add_key_value_pair(key, value, temp, last_locked, thread_num)) {
  	//If we are in this block, the child has split
  	oss.str("");
  	oss << "Locking node(" << this->unique_id << ") in add_to_child1" << endl;
  	safe_cout(oss.str(), thread_num);
  	
  	m.lock();
  	Inner_Node* tempNode;
  	Inner_Node* innerCopy;
  	  	
  	oss.str("");
  	//key -> child_split_key.key
  	oss << "Finding node to insert key " << temp.key << endl;
		safe_cout(oss.str(), thread_num);
		
  	//key -> child_split_key.key
		while(inner->high_key < temp.key && inner->high_key != -1){

			oss.str("");
			oss << "High key of node(" << inner->unique_id << ") = " << inner->high_key << endl;
			safe_cout(oss.str(), thread_num);
			
			if(inner->link_ptr != nullptr){
				oss.str("");
				oss << "Following link_ptr of node(" << inner->unique_id << ")" << endl;
				safe_cout(oss.str(), thread_num);
	
				tempNode = (Inner_Node*)inner->link_ptr;

				oss.str("");
		  	oss << "Locking node(" << tempNode->unique_id << ") in add_to_child2" << endl;
				safe_cout(oss.str(), thread_num);
	
				tempNode->m.lock();
				innerCopy = inner;
				inner = tempNode;

		  	oss.str("");
		  	oss << "Unlocking child node(" << innerCopy->unique_id << ") in add_to_child3" << endl;
		  	safe_cout(oss.str(), thread_num);
	
				innerCopy->m.unlock();	
			}
			else{
				//we've reached the rightmost leaf, so the value we are inserting must be the new high key
				break;
			}
		}
  	  	
  	//unlock child node
  	if(last_locked.is_leaf){
			Leaf_Node* locked_leaf = (Leaf_Node*)last_locked.node;

			oss.str("");
			oss << "Unlocking leaf(" << locked_leaf->unique_id << ") in add_to_child4" << endl;
			safe_cout(oss.str(), thread_num);
	
			locked_leaf->unlock_leaf();
  	}
  	else{
			Inner_Node* locked_node = (Inner_Node*)last_locked.node;
	
			oss.str("");
			oss << "Unlocking node(" << locked_node->unique_id << ") in add_to_child5" << endl;
			safe_cout(oss.str(), thread_num);
	
			locked_node->unlock_inner();  	
  	}
  	  	
  	if(key > inner->high_key){
  		inner->high_key = key;
  	}
  
  	//If this node is full, we will need to split it
  	if(inner->keys.size() == DATA_SLOTS){
  		child_split_key.key = temp.key;
  		child_split_key.node = temp.node;
  		last_locked.node = inner;
  		last_locked.is_leaf = false;

			oss.str("");
  		oss << "INNER NODE FULL AND SHOULD SPLIT WHEN " << temp.key << " IS ADDED\n";
			safe_cout(oss.str(), thread_num);
	
  		//return to calling function, passing new key and locked node
  		return true;
  	}
  
    /*  Search for the key.  If we can't find it, add to the end */ 
    auto keys_it = upper_bound(begin(inner->keys), end(inner->keys), key);
    if (keys_it == end(inner->keys)) {
    	oss.str("");
	   	oss << "INSERT NEW KEY AT END OF INNER NODE\n";
	   	safe_cout(oss.str(), thread_num);
	
      inner->keys.push_back(temp.key);
      inner->values.push_back(temp.node);
    }
    
    /*  If we can find it, that is where we have to insert */
    else {
  		oss.str("");
    	oss << "INSERT NEW KEY IN INNER NODE\n";
    	safe_cout(oss.str(), thread_num);  		
  
  		auto this_key = begin(inner->keys);
  		auto this_value = begin(inner->values);    
      for (; this_key != end(keys); this_key++, this_value++) {
		    if (*this_key > temp.key) {    	
		      inner->keys.insert(this_key, temp.key);
		   		break; 
		    }
	    }
	    
	    this_value++;
      inner->values.insert(this_value, temp.node);	    
    }
  }
  else{
  	oss.str("");
  	oss << "Locking node(" << inner->unique_id << ") in add_to_child6" << endl;	
  	safe_cout(oss.str(), thread_num);
	
  	inner->m.lock();
  	if(key > inner->high_key){
  		inner->high_key = key;
  	}

		oss.str("");
  	oss << "Child did not split\n";
  	safe_cout(oss.str(), thread_num);
	
  }
  oss.str("");
  oss << "Unlocking node(" << inner->unique_id << ") in add_to_child7" << endl;
  safe_cout(oss.str(), thread_num);
	
  inner->m.unlock();
  
  return false;
}


int Inner_Node::search(int key){
	auto value_it = values.begin();
	for(auto it = keys.begin(); it != keys.end(); it++, value_it++){
		if(*it >= key){
			//std::cout << "Following pointer left of key " << *it << endl;
			return (*value_it)->search(key);
		}
	}
	if(link_ptr != nullptr){
		//std::cout << "Following link pointer from node " << unique_id << endl;
		return link_ptr->search(key);
	}
	else{
		//std::cout << "Following end pointer of node " << unique_id << endl;
		Node* nodePtr = *(value_it);
		return nodePtr->search(key);
	}
}

void Inner_Node::check_keys(){

	ostringstream oss;
	int last_high = this->high_key;
	
  Inner_Node* temp = (Inner_Node*)this->link_ptr;
  while(temp != nullptr){
		if(last_high > temp->high_key){
			oss.str("");
			oss << "HIGH KEY ERROR IN INNER: " << last_high << " > " << temp->high_key << endl;
			safe_cout(oss.str(), 0);
			
			raise(SIGSEGV);
		}
		else{
			last_high = temp->high_key;
	  	temp = (Inner_Node*)temp->link_ptr;
		}
  }  	
  
  if(!values.empty()){
  	values.front()->check_keys();
  }
}


void Inner_Node::unlock_inner(){
	m.unlock();
}

void Inner_Node::lock_inner(){
	m.lock();
}

/*  Creates two leaf nodes, or each side of the first key.
    Only needs to be called once */
void Inner_Node::create_first_node(int key, int value){

  //To get things started, we need a leaf node on each side of the first key
  keys.push_back(key);
  Node* node1 = new Leaf_Node();
  Node* node2 = new Leaf_Node();
  node1->link_ptr = node2;

  values.push_back(node1);
  values.push_back(node2);
  this->high_key = key;
  
  //And then insert the new (key, value) on the left of the key
  Node_key temp;
	Node_key last_locked;
	(*begin(values))->add_key_value_pair(key, value, temp, last_locked, 0);
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

void Inner_Node::print_link(int depth){
	std::cout << "Printing level " << depth << endl;
	string padding(depth * 2, ' ');
  ostringstream oss;
  std::cout << padding << "Inner(" << unique_id << "): (";

  unsigned int counter = 1;
	
  for (auto& k : keys) {
    if(counter == keys.size())
    	std:: cout << k;
    	//oss << k;
    else
	    //oss << k << ", ";
	    std::cout << k << ", ";
    ++counter;
  }
  std::cout << "); High Key: " << this->high_key << endl;
	
  Inner_Node* temp = (Inner_Node*)this->link_ptr;
  while(temp != nullptr){
	  std::cout << padding << "Inner(" << temp->unique_id << "): (";
  	counter = 1;
  	for (auto& k : temp->keys) {
		  if(counter == temp->keys.size())
		  	std::cout << k;
		  else
			  std::cout << k << ", ";
		  ++counter;
		}
  	std::cout << ") High Key: " << temp->high_key << endl;
  	temp = (Inner_Node*)temp->link_ptr;
  }
  	
  
  if(!values.empty()){
  	values.front()->print_link(depth + 1);
  }
  	
}


string Inner_Node::print_r(int depth) {
  string padding(depth * 2, ' ');
  ostringstream oss;
  oss << padding << "Inner(" << unique_id << "): (";
  
  unsigned int counter = 1;
  for (auto& k : keys) {
    if(counter == keys.size())
    	oss << k;
    else
	    oss << k << ", ";
    ++counter;
  }
  oss << ") High Key: " << this->high_key << " ->{\n";
  for (auto& v : values) {
  	//Thing SEGFAULTed here
    oss << v->print_r(depth+1) << "\n";
  }
  oss << "} ";
  return oss.str();
}

