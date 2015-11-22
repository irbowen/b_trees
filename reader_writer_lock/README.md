## Plan
- I think dynamic locking is more work than its worth
- Each split has to actually touch the node anyway
- hash maps are expensive, and there could be a lot of conention on the lock table.
- I can create a read/writer lock for each node in the table 
  - Its an int, bool, mutex, and cv