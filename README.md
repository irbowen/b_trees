## TODOs
- Email IA
- Give ryan write permissions

## Concurrent B-Tree
- Relevant Articles
  - [Eff. locking](http://www.csd.uoc.gr/~hy460/pdf/p650-lehman.pdf)
  - [Lock free](http://www.cs.umanitoba.ca/~hacamero/Research/BtreeTechrpt2011.pdf)
  - [Cache oblivious](http://supertech.csail.mit.edu/papers/cobtree.pdf)
  - [Haven't looked at this one yet](http://koasas.kaist.ac.kr/bitstream/10203/18560/1/A%20Concurrent%20Blink-tree%20algorithm%20using%20a%20cooperative%20locking%20protocol.pdf)
- Types of trees
  - For each of these try, various numbers of threads with various read/write percentage workloads 
  - Try insert random, sorted, sorted desc
  - Lock-free
    - Various fanout    
  - Read/writer locker
    - Dynamic Lock allocation
      - Various fanout 
    - Static lock allocation
      - Various fanout 
  - Only exclusive lock ("control" case)
    - Dynamic Lock allocation
      - Various fanout 
    - Static lock allocation
      - Various fanout

## Progress so far
- Working on sequential tree
 
