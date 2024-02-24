### Problem 1
###### Method
In order to ensure that the count is accurate, I decided to leave the task of counting up to one thread, and have the remaining threads only supply a single cupcake on the first empty tray they see. This way, one guest can be sure all other guest have visited, based on the number of cupcakes they have eaten.

###### Correctness
Each guest will only have access to the cupcake state while the section is protected by a lock, similarly the accesses will be counted while protected by the lock. One consideration for correctness is avoiding additional counts to iterations for threads spinning to enter the labyrinth despite the minotaur being informed. In this case I prevented threads from updating the value if the minotaur has already been informed.

###### Effeciency
This strategy is very inefficient, as guest can re-enter and prevent everyone from accessing the labyrinth in a quick manner. Additionally, the cupcake eater has to wait their turn to enter at random, requiring a minimum number of n entries and the maximum being the program never actually completes.

###### Experimental Evaluation
In order to test the program I have the threads continually try to access the labyrinth, until a flag is set by the cupcake eater ensuring the run has concluded. While the treads are in the critical section they add to a total access counter, informing me how many total accesses were required for the threads to figure out they have all finished the labyrinth. I tested with 8, 10, 50, and 100 threads:
```
8 Threads: 0.001052 seconds and 7404 iterations.
10 Threads: 0.001138 seconds and 7801 iterations.
50 Threads: 0.00222 seconds and 19460 iterations.
100 Threads: 0.004033 seconds and 47794 iterations.
```
One thing to note is that the number of iteractions can vary greatly for the same number of threads, for instance I've had multiple runs at 100 threads take ~30k-50k iterations but had some with over 1 million iterations.

### Problem 2
###### Method
I chose protocol 3, because it is the strongest strategy for ensuring that every guest will visit the showroom and prevent slowdowns due to invalidations. This strategy can be represented by a queue lock, which I chose the array based lock for my implementation. To represent guest re-entering the queue, I have them generate a random number 0-5 to decide whether or not they will renter.

###### Correctness
This lock will ensure the critical section is protected it by binding it to method calls on the same object, preventing other threads from accessing volatile data at the same time. By spinning in the lock until the unlock function is called by the same thread, only one thread will have access to this shared data at a time.

###### Effeciency
This strategy is moderately effecient in terms of time, by tying the lock to a queue, it prevents mass invalidations of data. Although this implementation may be slower than CLH locks or MCS locks, because of reseting the cache line, it will still have a comparable time. The biggest downside to the array queue is the additional memory it requires, and in the case you want to avoid cache line refreshes, the queue needs to be built with padding, costing a signficant amount of additional memory.

###### Experimental Evaluation
In order to test the my array queue I have test the amount of time it takes for 100, 1000, 10000, 100000 threads to completly exhasut the queue. Times I got while testing are as follows:
```
100 threads: 0.002994 seconds
1000 threads: 0.029635 seconds
10000 threads: 0.44082 seconds
100000 threads: 29.4404 seconds
```

From these tests we see that the threads increase over time where the time/thread has a slope that is greater than linear.
This is mostly seen in the jump from 10,000 threads to 100,000 where it takes roughly 67x longer, compared to the other jumps much which are about to 10x and 15x repectively.