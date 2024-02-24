// Mathew Reilly
// COP4520
// Assignment 2
// Description:
//    .
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <vector>
#include <chrono>
using namespace std;


/*
Problem 1:
The guests may enter the labyrinth, one at a time, and only when he invites them to do so.

When a guest reaches the end of the labyrinth, they may decide to eat the cupcake or leave it.

If a cupcake was eaten by a previous guest, the plate will be empty when the next guest arrives, and
they may requst for a cupcake.

When the new cupcake is brought, that guest may eat it or leave it on the plate.

Withought allowing the guest to talk to eachother once the game has started, how can they let the Minataur know they all
entered the labyrinth.

Simulate the winning protocol.
*/


/*
How should every guest behave so that only by viewing the platter they know that every other guest has gone through
the labyrinth?

Lets start with the perspective of the first guest. As the first guest, the problem cannot advance without taking a cupcake.
If a cupcake does not get taken, the next guest has gained nothing.

So the first thread will take a cupcake, leaving the tray empty for the next guest.

Potential actual strategy:
  One thread/guest is assigned to cupcake eating, and is aware of the total number of guests before beginning. When
  this thread reaches the end of the labyrinth, it will always eat a cupcake if it is there, keeping count. This thread,
  will never place a cupcake.

  Every other thread, is allowed to place one cupcake total, once they encounter an empty tray.

  Once the first thread, has eaten all N cupcakes, it know that everyone has entered at least once. allowing that guest to
  inform the minotaur that everyone has entered the labyrinth.

Idea 1)

  If this first guest gets called again, as the second to enter the maze, when they reach the end they should
  leave the tray empty.

  If the guest is a unique guest, they should both request a cupcake and not eat it?

  If a repeated guest enters, they leave the state of the cupcake.

  When the next unique guest encounters a cupcake, they will eat it leaving the tray empty.

  Every-non unique guest will leave the state alone.
  If you 

Idea 2) NO

  Each guest counts the number of times they have completed the labyrinth, the first time, if the tray is empty they will add a cupcake
  and not eat it. (IE always leave the cupcake).

  The second time they will eat the cupcake.

  The third and onward time they will always leave the cupcake for many iterations

  The "last" they will eat the cupcake,


Idea 3)

  

*/


mutex inLabyrinth;
bool hasCupcake;
bool minotaurInformed;


void guest(int type, int number)
{
  counter = 0;
  inLabyrinth.lock();
    if(type == 1)
    {
      // eat the cupcake, add to count.
      hasCupcake = false;
      counter++;

    } else 
    {
      // if not cupcake eater, check if plate is empty and has a cupcake to give.
      if(counter < 1 && hasCupcake == false)
      {
        // give available cupcake.
        hasCupcake == true;
        counter++;
      }
    }
  inLabyrinth.unlock();
  
  if(counter == number)
  {
    minotaurInformed = true;
  }

  while(counter != number || !minotaurInformed)
  {

  }
}


int main()
{
  hasCupcake = true;
  minotaurInformed = false;

  vector<thread> threads;
  ArrayLock = cupcakeLock();

  int numberGuests = 0;
  cout << "Please Enter the Number of Guests: " << endl;
  cin>>numberGuests;

  // start the clock and run the threads to completion
  auto start = chrono::high_resolution_clock::now();

  for(int i = 0; i < numberGuests; i++)
  {
    treads.emplaceBack(guest, cupcakeLock, i);
  }

  for(auint i = 0; i < numberGuests; i++)
  {
    threads[i].join();
  }

  auto stop = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

  // calculate time in seconds
  double timeSeconds = duration.count() / 1000000.0;


  return 0;
}


