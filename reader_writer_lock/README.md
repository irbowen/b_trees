## Reader Writer Lock Tree
- This tree uses reader (shared) and writer (exclusive) locks for concurrency management.
- It uses the "can split" idea on child nodes to determine what lock to hold over the insert operation


- Sibiling pointers on the leaf nodes.  This dramitically speeds up sequential reads (because you don't have to go back up the tree, or traverse from the top again - you find one leaf node and go from there).  For writes, it imposes a cost.  Everytime a leaf node splits, the pointers must be updates.  In a concurrent enviornment, the overhead from this becomes more dramtic.
- Consistent Depth: This is closely coupled with the sibiling pointer.  If the depth is no longer consitent, and thamaintaing sibling pointers is more difficult - you have to traverse the tree to update the pointers, so you might as well just traverse the tree when you're reading it.  However, writers could benefit from this.  Rather than paying the cost of traversing all the nodes and them potentiall splitting, we can just find where we want to insert and put it in.  Nothing wrong with the Root node, or any inner node, having a leaf node as a child
