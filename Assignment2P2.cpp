// Mathew Reilly
// COP4520
// Assignment 2
// Description:
//    .
#include <iostream>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <chrono>
using namespace std;


/*
Problem 2:
Pick one of the 3 strategies/protocol to enter a showroom one at a time to implement.

1) Guest can check to see if the showroom door is open and try to enter the room.

2) Allows guest to flip a sign at the door indicating "busy" or "available". guest must set the showroom to busy when entering
available while leaving.

3) Guest would line in a queue. Every guest exiting the room is responsible for notifying guest standing in front of the queue
that the showroom is available. Guest can queue multiple times.

Which strategy will you choose and why? - what are the advantages and disadvantages

- The guests should choose strategy 3, because forming a queue will be the most effecient way for everyone to see the room.
Strategies 1 and 2 run into the problem of starving out some of the guests, preventing them from entering the showroom.

- The issue with strategy 1 is having the guest checking to see if the showroom is open is a matter of the
 test and set lock. Trying to continually acquire the showroom causes all of the threads to be waiting outside the door,
 and when one finally is let in, the remaining ones that try to enter will be invalidated and incur a time penalty. Not only
 does this time penalty occur, but some guests/threads may never enter the showroom as they are starved out.
 
- The issue is similar with stragety 2, behaving similarly to the test and test and set locks. In this implementation, 
  the guest are constantly waiting for the room to be open, pouncing at the first opportunity. Again, waves of threads will
  try to enter ad the same time, causes mass invalidations. Although less of the threads are likely to collide and slow things
  down, and it is an improvement over normal test and set, when there are many guest the implementation slows down significantly.
  Again, guests/threads can be starved out.

- Strategy 3 can completely avoid the invalidations created by strategy 1 and 2. By queuing the threads, they will avoid competing
  for the show room and not create these invalidations. Additionally this implementation helps avoid starving the threads,
  as long as a thread is queued it will eventually make it to the show room. The biggest drawback with this queue implementation
  is that it requires additional memory and structural support. Will require memory in managing them and in some cases empty elements
  to prevent cache lines being overwritten.

*/

mutex arrLockMutex;

thread_local int myGuestIndex;

class ArrayLock
{
  private:
    int tail;
    int size; // keeps track of the number of guests
    vector<bool> turnTracker; // false means it is yet to be the threads turn, does it need to be marked volitile???

  public:
    ArrayLock(int size)
    {
      // do I need to lock this section?
      this->size = size;
      tail = 0;
      turnTracker.resize(size);
      turnTracker[0] = true;
    }

    void lock()
    {
      // arrLockMutex.lock(); // do I need to mutex this, it is critical but won't this function lock on it's own?
      int guestIndex = tail % size;
      tail++;
      myGuestIndex = guestIndex;
      //cout << myGuestIndex << " " << tail << endl;
      while(!turnTracker[guestIndex]){};
    }

    void unlock()
    {
      int guestIndex = myGuestIndex;
      turnTracker[guestIndex] = false;
      //arrLockMutex.unlock(); Is this another critical section thing?
      turnTracker[(guestIndex + 1) % size] = true;
      
    }

};

// For this problem I will use a queue lock. (not sure which type will best reflect requirements)

// to implement this lock first there needs to be a way to represent the guests as threads, where each guest 
// will hold a lock.

// cupcake represents lock???
void guest(ArrayLock *showRoomLock, int number, int requeueCount)
{
  while(requeueCount > 0)
  {
    showRoomLock->lock();
    // not sure if this is really reflective of the same guest every time they enter....
    //cout << "guest " << number << " has accessed the showroom. ";
    showRoomLock->unlock();
    //cout << myGuestIndex << endl;
    requeueCount--;
  }
  
}


int main()
{
  vector<thread> threads;

  int numberGuests = 0;
  cout << "Please Enter the Number of Guests: " << endl;
  cin>>numberGuests;

   ArrayLock showRoomLock(numberGuests);

  // start the clock and run the threads to completion
  auto start = chrono::high_resolution_clock::now();

  for(int i = 0; i < numberGuests; i++)
  {
    threads.emplace_back(guest, &showRoomLock, i, rand() % 5);
  }

  for(int i = 0; i < numberGuests; i++)
  {
    threads[i].join();
  }

  auto stop = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

  // calculate time in seconds
  double timeSeconds = duration.count() / 1000000.0;

  cout << timeSeconds << " seconds to appease all " << numberGuests << " guests that were allowed to queue up to 5 times." << endl;

  return 0;
}