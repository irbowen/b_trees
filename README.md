## Notes
- I moved the read/write lock and dynamic locking files into a new folder, and copied the rest of the files.  
- I'll be starting from here
- I didn't implement the get(key) for the sequential version... We'll have to each write from scratch
- Behavior for duplicate keys is undefined... and I think we should leave it that way.  Lets us increase performance
  - If a inner node splits on 
  - [44, 44, 44, 44, 56, 56] => [44,44,44] [44,56,56]
  - Insert 45 from child split
  - [44,44,44, 45] [44,56,56]
  - But fixing this means we have to scan ahead to the next inner node, or check the children...
  - Both are expensive, so I say we either
    - Leave the behavior undefined
    - Check when inserting if the key exists and fail silently
    - Check when inserting if the key exists and update value

## TODOs
- Read over papers again

## Concurrent B-Tree
- Relevant Articles
  - [Eff. locking](http://www.csd.uoc.gr/~hy460/pdf/p650-lehman.pdf)
  - [Lock free](http://www.cs.umanitoba.ca/~hacamero/Research/BtreeTechrpt2011.pdf)
  - [Cache oblivious](http://supertech.csail.mit.edu/papers/cobtree.pdf)
  - [Haven't looked at this one yet](http://koasas.kaist.ac.kr/bitstream/10203/18560/1/A%20Concurrent%20Blink-tree%20algorithm%20using%20a%20cooperative%20locking%20protocol.pdf)

