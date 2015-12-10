## Usage
In each of the implementation folders, you type "make" to build the executable.
From there, you can run any of the test cases found in tests/ with a command like

  ./Trees < ../tests/test1.txt

There are a variety of test cases available that we used to build our datasets.

## Source Code

The source code for the sequential versions, reader writer lock array, and reader writer lock list are all very similair.
You can find the code for each in the directory with the matching name.
Each has the same basic idea:
  Tree: Manages the root node
  Inner_Node: Contains no data, just indexes to children
  Leaf_Node: Contain data

Calling insert() or get_value() or the tree calls the same operation on the root node, which then calls the same thing on its children.

In each variation, Inner_Node::add_key_value_pair, Inner_Node::add_to_child(), and Leaf_Node::add_key_value_pair() are the most interesting functions, and contain much of the logic and locking.
The main.ccp file shows how the tests were conducted.
