### Problem 1
###### Method
In order to ensure that the count is accurate, I decided to leave the task of counting up to one thread, and have the remaining threads only supply a single cupcake on the first empty tray they see. This way, one guest can be sure all other guest have visited, based on the number of cupcakes they have eaten.

###### Correctness
Need to update ___________________ (Focus on safety of critical sections and why output should be correct)

###### Effeciency
This strategy is very inefficient, as guest can re-enter and prevent everyone from accessing the labyrinth in a quick manner. Additionally, the cupcake eater has to wait their turn to enter at random, requiring a minimum number of n entries and the maximum being the program never actually completes. The expected amount of times threads will enter the maze is roughly ____?

###### Experimental Evaluation
In order to test the program I have the threads continually try to access the labyrinth, until a flag is set by the cupcake eater ensuring the run has concluded. While the treads are in the critical section they add to a total access counter, informing me how many total accesses were required for the threads to figure out they have all finished the labyrinth. I tested with 8, 10, 13 threads, getting _________, _________, and _________ accesses repectively. In reality we see the relationship to number of threads and number of access is ________.

### Problem 2
###### Method
I chose protocol 3, because it is the strongest strategy for ensuring that every guest will visit the showroom and prevent slowdowns due to invalidations. This strategy can be represented by a queue lock, which I chose the array based lock for my implementation. To represent guest re-entering the queue, I have them generate a random number 0-5 to decide whether or not they will renter.

###### Correctness
This lock will ensure the critical section is protected it by binding it to method calls on the same object, preventing other threads from accessing volatile data at the same time. By spinning in the lock until the unlock function is called by the same thread, only one thread will have access to this shared data at a time.

###### Effeciency
This strategy is moderately effecient in terms of time, by tying the lock to a queue, it prevents mass invalidations of data. Although this implementation may be slower than CLH locks or MCS locks, because of reseting the cache line, it will still have a comparable time. The biggest downside to the array queue is the additional memory it requires, and in the case you want to avoid cache line refreshes, the queue needs to be built with padding, costing a signficant amount of additional memory.

###### Experimental Evaluation
In order to test the my array queue I have test the amount of time it takes for 100, 1000, 10000, 100000 threads to completly exhasut the queue. Times I got while testing are as follows:
100 threads: 0.002994 seconds
1000 threads: 0.029635 seconds
10000 threads: 0.44082 seconds
100000 threads: 29.4404 seconds
From these tests we see that the threads increase over time where the time/thread has a slope that is greater than linear.
This is mostly seen in the jump from 10,000 threads to 100,000 where it takes roughly 67x longer, compared to the other jumps much which are about to 10x and 15x repectively.