// Mathew Reilly
// COP4520
// Assignment 2
// Description:
//    This code follows the second part of problem 2, implementing a queue based strategy allowing guest to see a one person at
//    a time showroom. My implementation follows the ArrayLock where threads inform eachother through a boolean array.
#include <iostream>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <chrono>
#include <fstream>
using namespace std;

// This declares a thread local variable that will keep track of a threads index.
thread_local int myGuestIndex;

// This class implements an ArrayLock which is a queue based lock. In it the threads are queued based on their index and
// informed of their turn through a boolean array that is set by the predecessor node.
class ArrayLock
{
  private:
    int tail;
    int size; // keeps track of the number of guests
    vector<bool> turnTracker; // false means it is yet to be the threads turn, true means it is that threads turn

  public:
    // constructor to initialize the lock
    ArrayLock(int size)
    {
      // do I need to lock this section?
      this->size = size;
      tail = 0;
      turnTracker.resize(size);
      turnTracker[0] = true;
    }

    // This will prevent other threads from accessing the lock as only one thread will have access to the object, and hold onto
    // it until the thread relinquish it.
    void lock()
    {
      // set the current tail thread based on the current tail position, increase the tail position.
      int guestIndex = tail % size;
      tail++;
      myGuestIndex = guestIndex;
      while(!turnTracker[guestIndex]){}; // holds onto the lock until relinquished by thread.
    }

    // Will release the lock and allow the next thread to begin.
    void unlock()
    {
      int guestIndex = myGuestIndex;
      turnTracker[guestIndex] = false;
      turnTracker[(guestIndex + 1) % size] = true;
    }

};

// Each thread will have access to the same lock object, the array lock, a number representing that guest and the number of times
// that thread decides to requeue.
void guest(ArrayLock *showRoomLock, int number, int requeueCount)
{
  // This allows threads with a requeueCount > 0 to take another position in the queue.
  while(requeueCount > 0)
  {
    showRoomLock->lock();
      // guest enters the showroom and views it
      // when the guest exists they inform the next person in queue
    showRoomLock->unlock();
    requeueCount--;
  }
  
}


int main()
{
  // array to hold all of the threads
  vector<thread> threads;

  // takes input on number of guests/threads
  int numberGuests = 0;
  cout << "Please enter the number of guests: " << endl;
  cin>>numberGuests;

  // the shared lock is created here
  ArrayLock showRoomLock(numberGuests);

  // start the clock and run the threads to completion
  auto start = chrono::high_resolution_clock::now();

  // each guest is created and the number of times they queue is decided.
  for(int i = 0; i < numberGuests; i++)
  {
    threads.emplace_back(guest, &showRoomLock, i, rand() % 5);
  }

  // makes sure every guest finishes viewing
  for(int i = 0; i < numberGuests; i++)
  {
    threads[i].join();
  }

  auto stop = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

  // calculate time in seconds
  double timeSeconds = duration.count() / 1000000.0;

  // output for that run is output to a file.
  ofstream showroomPrintFile("P2Result.txt");
  showroomPrintFile << timeSeconds << " seconds to appease all " << numberGuests << " guests that were allowed to queue up to 5 times." << endl;
  showroomPrintFile.close();

  return 0;
}