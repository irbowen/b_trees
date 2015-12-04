## Plan
- I think dynamic locking is more work than its worth
- Each split has to actually touch the node anyway
- hash maps are expensive, and there could be a lot of conention on the lock table.
- I can create a read/writer lock for each node in the table 
  - Its an int, bool, mutex, and cv


## Paper

Concurrent B-trees (and their various other forms: B+trees, B** Trees, B-link tress, etc) all have one thing in common - they are optimized for reads.  

At the dawn of database systems, these data structures were choosen for indexes because each node could be made to fit within a single disk page - the structures advantage was that it worked very well with block oriented secondary storage.  As main memories grew, this became less and less of a problem.  In 2015, most "normal" databases can easily fit in the main memory of a large server machine.  B-Trees and their varients are still used because they still provide some advantages compared to hashing or other forms of indexing SUCH AS WHAT?  Namely, B-Trees keep their elements in order, so accessing the next element is very cheap.

Most B-Trees are now a variant of B+Trees, in that they keep all of their data in the leaf nodes.  This is good for several reasons.  It allows a clear distinction between inner and leaf nodes - inner nodes hold keys, leaf nodes hold data.  It also allows you to only traverse down to the element you're looking for in the leaf node, and then you can just read across, without having to go back up.  This is assuming that all the leaf nodes are at the same level, and the depth of the tree is consistent.  

However, these traits are important to reads - what about writes?  If we wanted to create a tree that was good at writing very quickly, how would we change it?  What if we were looking to support multiple, concurrent writers?  This complicates the problem.

However, we can improve the performance of the tree by removing several of the principles that make trees so good for reading data.  THAT THEY ARE BALANCE, ALL LEAFS SAME LEVEL
What about writes though?  They don't benefit from all of the features that we've added to speed up reads.  
For example
- Sibiling pointers on the leaf nodes.  This dramitically speeds up sequential reads (because you don't have to go back up the tree, or traverse from the top again - you find one leaf node and go from there).  For writes, it imposes a cost.  Everytime a leaf node splits, the pointers must be updates.  In a concurrent enviornment, the overhead from this becomes more dramtic.
- Consistent Depth: This is closely coupled with the sibiling pointer.  If the depth is no longer consitent, and thamaintaing sibling pointers is more difficult - you have to traverse the tree to update the pointers, so you might as well just traverse the tree when you're reading it.  However, writers could benefit from this.  Rather than paying the cost of traversing all the nodes and them potentiall splitting, we can just find where we want to insert and put it in.  Nothing wrong with the Root node, or any inner node, having a leaf node as a child
