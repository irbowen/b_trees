#include "leaf_node.h"

using namespace std;

/*  Create a new Leaf_Node.  At this point, sibling pointers don't need to 
    point to anything */
	Leaf_Node::Leaf_Node() {
	link_ptr = nullptr;
  unique_id = Node::get_counter();
  high_key = -1;
  is_leaf = true;
}

bool Leaf_Node::can_split() {
  if (elements.size() >= DATA_SLOTS) {
    return true;
  }
  return false;
}

bool Leaf_Node::add_key_value_pair(int key, int value, Node_key& node_key, Node_key& last_locked, int thread_num) {
  /* We're clearly here to do a write */
  
  ostringstream oss;
  oss << "Locking leaf(" << unique_id << ")" << endl;
  safe_cout(oss.str(), thread_num);
  
	m.lock();

	/*Make sure this is correct node to insert into*/
	Leaf_Node* leaf = this;
	Leaf_Node* leaf_copy;
	Leaf_Node* tempLeaf;

	oss.str("");
	oss << "Finding node to insert key " << key << endl;
	safe_cout(oss.str(), thread_num);
	
	while(leaf->high_key < key && leaf->high_key != -1){
	
		oss.str("");
		oss << "High key of leaf(" << leaf->unique_id << ") = " << leaf->high_key << endl;
		safe_cout(oss.str(), thread_num);
	
		if(leaf->link_ptr != nullptr){
			oss.str("");
			oss << "Following link_ptr of leaf(" << leaf->unique_id << ")" << endl;
			safe_cout(oss.str(), thread_num);
		
			tempLeaf = (Leaf_Node*)leaf->link_ptr;
		  
		  oss.str("");
		  oss << "Locking leaf(" << tempLeaf->unique_id << ")" << endl;
			safe_cout(oss.str(), thread_num);
			
			tempLeaf->m.lock();
			leaf_copy = leaf;
			leaf = tempLeaf;
		  
		  oss.str("");
		  oss << "Unlocking leaf(" << leaf_copy->unique_id << ")" << endl;
			safe_cout(oss.str(), thread_num);
			
			leaf_copy->m.unlock();	
		}
		else{
			//we've reached the rightmost leaf, so the value we are inserting must be the new high key
			break;
		}
	}
	
	oss.str("");
	oss << "Inserting into leaf(" << leaf->unique_id << ")" << endl;
	safe_cout(oss.str(), thread_num);
	
	for (auto check_it = elements.begin(); check_it != elements.end(); check_it++) {
    if(get<0>(*check_it) == key){
		  oss.str("");
			oss << "Key " << key << " already present. Unlocking and returning\n";
			safe_cout(oss.str(), thread_num);
	  
	  	leaf->m.unlock();
	  	return false;
    }
  }
	
  /*  But what if we need to split the node? */
  if (leaf->elements.size() == DATA_SLOTS) {

		oss.str("");
  	oss << "SPLITTING\n";
		safe_cout(oss.str(), thread_num);
	  	
    /*  Find the midpoint element in the list.... Linear search */
    auto mid_point = leaf->elements.size() / 2;
    auto it = begin(leaf->elements);
    for (size_t i = 0; i < mid_point - 1; i++) {
      it++;
    }

    /*  So then this is our key */
    auto new_key = *it;
		
		oss.str("");
		oss << "NEW KEY = " << get<0>(new_key) << endl;
		safe_cout(oss.str(), thread_num);
	
    /*  And we want our key to have its descendants on the left. */
    it++;
    list<tuple<int, int>> right(it, end(leaf->elements));

    /*  Resize, since we're giving half of our list to the new node */
    leaf->elements.resize(mid_point);
    
    //Add new value to correct node
    if(key <= get<0>(new_key)){
    	
    	oss.str("");
    	oss << "Add to left split node\n";
    	safe_cout(oss.str(), thread_num);
	
			leaf->elements.push_back(make_tuple(key, value));

			leaf->elements.sort([](auto& a, auto& b) {
				return get<0>(a) < get<0>(b);
			});			
    }
    else{
    	
    	oss.str("");
    	oss << "Add to right split node\n";
    	safe_cout(oss.str(), thread_num);
    	
    	right.push_back(make_tuple(key, value));

			right.sort([](auto& a, auto& b) {
				return get<0>(a) < get<0>(b);
			});
    }	
        
    /*  Set the node_key to pass back up */
    
    Leaf_Node* temp = new Leaf_Node();
    
    node_key.key = get<0>(new_key);
    node_key.node = temp;

		//Set new node link pointer to old link pointer
		temp->link_ptr = leaf->link_ptr;
		
		//Set old node link pointer to new node
		leaf->link_ptr = reinterpret_cast<Leaf_Node*>(node_key.node);

    //Set high keys
		temp->high_key = std::max(high_key, key);
		leaf->high_key = get<0>(new_key);	
			
		//Add elements to new node
    temp->add_vector(right);

    /* Pass pointer to this node back to parent so that parent can be locked before we unlock */
    last_locked.node = leaf;
    last_locked.is_leaf = true;
		//leaf->m.unlock();
    return true;
  }
  else{
		leaf->elements.push_back(make_tuple(key, value));
		leaf->elements.sort([](auto& a, auto& b) {
			return get<0>(a) < get<0>(b);
		});
		
		if(key > leaf->high_key){
			leaf->high_key = key;
		}

	  oss.str("");
	  oss << "Unlocking leaf(" << leaf->unique_id << ")" << endl;
		safe_cout(oss.str(), thread_num);
		
		leaf->m.unlock();
		return false;
	}
}


int Leaf_Node::search(int key){
	
	for(auto it = elements.begin(); it != elements.end(); it++){
		if(get<0>(*it) == key){
			//cout << "Returning pair <" << get<0>(*it) << "," << get<1>(*it) << "> of leaf node " << unique_id << endl;
			return get<1>(*it);
		}
		else if(get<0>(*it) > key){
			//TODO Fix this
			
			//std::cout << "Reached key " << get<0>(*it) << " indicating value  not found\n";
			return -1;
		}
	}

	return link_ptr->search(key);
}



/*  Copies all the elements of the vector into the node's internal storage
    TODO: make this a move, &&, or something else fancy/fast 
    NOTE: NOT THREAD SAFE */
void Leaf_Node::add_vector(list<tuple<int, int>> v) {
  for (auto& i : v) {
    elements.push_back(i);
  }
}

void Leaf_Node::unlock_leaf(){
	this->m.unlock();
}

/*  Prints all the keys of the elements stored in this leaf node for debugging 
    NOTE: NOT THREAD SAFE*/
string Leaf_Node::print_r(int depth) {
  ostringstream oss;
  string padding(depth * 2, ' ');
  oss  << padding << "Leaf(" << unique_id << "): [";
  oss << print_keys();
  oss << "] High Key: " << high_key;
  return oss.str();
}

void Leaf_Node::print_link(int depth){
	std::cout << "Printing leaf nodes\n";
  ostringstream oss;
  string padding(depth * 2, ' ');
  std::cout  << padding << "Leaf(" << unique_id << "): [";
  std::cout << print_keys();
  std::cout << "] High Key: " << high_key << "\n";
  
  Leaf_Node* temp = (Leaf_Node*)this->link_ptr;
  while(temp != nullptr){
		std::cout  << padding << "Leaf(" << temp->unique_id << "): [";
		std::cout << temp->print_keys();
		std::cout << "] High Key: " << temp->high_key << "\n";	
		
		temp = (Leaf_Node*)temp->link_ptr;
  }
  
	return;
}

void Leaf_Node::check_keys(){
	ostringstream oss;
	int last_high = this->high_key;
	
  Leaf_Node* temp = (Leaf_Node*)this->link_ptr;
  while(temp != nullptr){
		if(last_high > temp->high_key){
				oss.str("");
				oss << "HIGH KEY ERROR IN LEAF: " << last_high << " > " << temp->high_key << endl;
				safe_cout(oss.str(), 0);
		
				raise(SIGSEGV);
		}
		else{
			last_high = temp->high_key;
	  	temp = (Leaf_Node*)temp->link_ptr;
		}
  }  	
  
  return;
}

/*  Print all the keys in stored in this leaf node 
    NOTE: NOT THREAD SAFE*/
string Leaf_Node::print_keys() {
  ostringstream oss; 
  unsigned int counter = 1;
  for (auto& e : elements) {
  	if(counter == elements.size())
  		oss << get<0>(e);
  	else
	    oss << get<0>(e) << ", ";
	  ++counter;
  }
  return oss.str();
}
