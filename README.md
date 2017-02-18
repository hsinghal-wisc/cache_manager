# cache_manager
I have implemented Cache Manager in C. I couldnt't compile it properly in given time frame.

Procedure followed:
I modified the "multiplereader-singlewriter-readerpriority-producerconsumer" algo to fit the cache manager problem.
On top of it, then I tried to implement LRU cache using LinkedList.

To run the makefile:

Please download the test folder and run the following commands:
1. make clean
2. make
3. ./server 5003 8 16  
The above command is in format : server [portnum] [threads] [buffers] 
Then on local web browser : run 127.0.0.1:5003 (not compiling, seems to hang)

Points to be noted regarding submitted code:

1. Satisfied - Writers/readers are random and can make requests at any time
2. Satisfied - Strong consistency guarantees: A reader never reads stale data (data written to already)
3. Readers get preference over writers, multiple readers-one writer at a time. 
4. But Readers will not get notified if the data they have, has gone stale until they re-read.
5. Could not implement a distributed system.
6. However no policy to prevent starvation.
7. Problems 4 and 5 could be solved together by implementing AFS/NFS type of policies over implemented code
8. Problem 6 could be solved by implementing dining philosopher on top of the existing code.
