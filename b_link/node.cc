#include "node.h"

int Node::counter = 0;

bool link_ptr = nullptr;

int Node::get_counter() {
  return counter++;
}

int Node::search(int key){
	std::cout << "WHY ARE WE IN THE SEARCH FUNCTION FOR NODE.CC?\n";
	return key;
}
