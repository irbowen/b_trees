- Design Assumptions
  - Assumed fan_out would be high
  - Vectors so we have O(1) access to midpoint, and binary_search to find where to insert
  - If fan_out is small, linked lists with linear search might tbe faster
  
- Reader Writer Locks
  - Reader locks are essentialy just couters - just so we know not to split a node its in use
  - But our project is for heavy inserts, so...
  - On every inserts, you have to lock every node on the way down, because a split could propagate all the way back up
  - This createss a lot of contention on the root node - becomes a serious bottleneck
  
- Baisc Ideas, Overview:
  - Locks vs Lock free
  - Pessimistic vs Optimisitc
  - Test and set vs compare and swap
  - Locks => traffic lights
  - Atomics/CAS => cloverleaf [http://d3z1rkrtcvm2b.cloudfront.net/wp-content/uploads/2014/06/138_1nc_airl8852.jpg]
  - All shared mutable state must be protect by an atomic or lock
