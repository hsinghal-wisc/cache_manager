# cache_manager
I have implemented Cache Manager in C. I couldnt't compile it properly in given time frame.

Procedure followed:
I modified the "multiplereader-singlewriter-readerpriority-producerconsumer" algo to fit the cache manager problem.
On top of it, then I tried to implement LRU cache using LinkedList.

To run the makefile:
Points to be noted regarding submitted code:

1. Satisfied - Writers/readers are random and can make requests at any time
2. Satisfied - Strong consistency guarantees: A reader never reads stale data (data written to already)
3. Readers get preference over writers, multiple readers-one writer at a time. 
4. But Readers will not get notified if the data they have, has gone stale until they re-read.
5. Could not implement a distributed system.
6. However no policy to prevent starvation.
7. Problems 4 and 5 could be solved together by implementing AFS/NFS type of policies over implemented code
