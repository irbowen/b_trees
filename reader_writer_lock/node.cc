#include "node.h"

int Node::counter = 0;

int Node::get_counter() {
  return counter++;
}
