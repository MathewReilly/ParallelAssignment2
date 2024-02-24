### Problem 2 Part 1 - Explanation:
Pick one of the 3 strategies/protocol to enter a showroom one at a time to implement.

1) Guest can check to see if the showroom door is open and try to enter the room.
2) Allows guest to flip a sign at the door indicating "busy" or "available". Guest must set the showroom to busy when entering available while leaving.
3) Guest would line in a queue. Every guest exiting the room is responsible for notifying guest standing in front of the queue that the showroom is available. Guest can queue multiple times.

Which strategy should the guest choose and why? - what are the advantages and disadvantages

- The guests should choose strategy 3, because forming a queue will be the most effecient way for everyone to see the room Strategies 1 and 2 run into the problem of starving out some of the guests, preventing them from entering the showroom if they choose to re-enter, as well as cause mass invalidations when rushing to enter.

- The issue with strategy 1 is having the guest checking to see if the showroom is open is a matter of the test and set lock. Trying to continually acquire the showroom causes all of the threads to be waiting outside the door, and when one finally is let in, the remaining ones that try to enter will be invalidated and incur a time penalty. Not only does this time penalty occur, but some guests/threads may never enter the showroom as they are starved out.
 
- The issue is similar with stragety 2, behaving similarly to the test and test and set locks. In this implementation, the guest are constantly waiting for the room to be open, pouncing at the first opportunity. Again, waves of threads will try to enter ad the same time, causes mass invalidations. Although less of the threads are likely to collide and slow things down, and it is an improvement over normal test and set, when there are many guest the implementation slows down significantly. Again, guests/threads can be starved out.

- Strategy 3 can completely avoid the invalidations created by strategy 1 and 2. By queuing the threads, they will avoid competing for the show room and not create these invalidations. Additionally this implementation helps avoid starving the threads, as long as a thread is queued it will eventually make it to the show room. The biggest drawback with this queue implementation is that it requires additional memory and structural support. Will require memory in managing them and in some cases empty elements to prevent cache lines being overwritten.
