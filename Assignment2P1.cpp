// Mathew Reilly
// COP4520
// Assignment 2
// Description:
//    This program solves a problem of having guest being supervised by a minotaur such that guests enter a 
//    labyrinth when called. At the exit of the labyrinth, there is a platter with a cupcake. A guest who reached the end
//    may choose to eat the cupcake, leaving the tray empty. If a guest enters and sees an empty platter, they may request
//    another cupcake, which they may choose to eat or leave.
//
//    How can the guests create a strategy that informs the minotaur that every guest has entered the labyrinth?
//      - Guest may not communicate once they begin entering the labyrinth.
//
//    My prgram works by following this logic:
//    One thread/guest is assigned to cupcake eating, and is aware of the total number of guests before beginning. When
//    this thread reaches the end of the labyrinth, it will always eat a cupcake if it is there, keeping count. This thread,
//    will never place a cupcake.
//
//    Every other thread, is allowed to place one cupcake total, once they encounter an empty tray.
//
//    Once the first thread, has eaten all N cupcakes, it know that everyone has entered at least once. allowing that guest to
//    inform the minotaur that everyone has entered the labyrinth.
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <vector>
#include <chrono>
#include <fstream>
using namespace std;

// Variables for a lock on global data, the cupcake state, whether or not the minotaur has been informed, and the total
// number of times guests have entered the maze.
mutex inLabyrinth;
bool hasCupcake;
bool minotaurInformed;
int totalAccesses;


// threads represent guest. Guest will have a type and number, where they eitehr a cupcake eater or placeer and
// are aware of the total number of guests participating. They can enter the labyrith by acquiring the lock and
// perform an action associated with their type.
void guest(int type, int number)
{
  int counter = 0;
  while(!minotaurInformed)
  {
    // guest enters the labyrinth, meaning that only they have information on and can influence the cupcake state.
    inLabyrinth.lock();
    // while the minotaur has not been informed add to the accesses, otherwise ignore uneeded accesses.
    if(!minotaurInformed)
    {
      totalAccesses++;
    }
    
    // If the guest is the cupcake eater, eat the cupcake and increase the counter.
    if(type == 1)
    {
      hasCupcake = false;
      counter++;

    } else 
    {
      // if not cupcake eater, check if plate is empty and has a cupcake to give.
      if(counter < 1 && hasCupcake == false)
      {
        // give available cupcake.
        hasCupcake = true;
        counter++;
      }
    }
    inLabyrinth.unlock();
    
    // Once all N cupcakes have been eaten, announce this to the minotaur, no longer requiring threads to enter the labyrinth.
    if(counter == number)
    {
      minotaurInformed = true;
    }
  }
}


int main()
{
  // initializing variables for the threads to interact with
  hasCupcake = true;        // inital state of cupcake
  minotaurInformed = false;
  totalAccesses = 0;

  // will be responsible for holding our total number of threads
  vector<thread> threads;

  int numberGuests = 0;
  cout << "Please enter the bumber of guests: " << endl;
  cin>>numberGuests;

  // start the clock and run the threads to completion
  auto start = chrono::high_resolution_clock::now();

  // create threads, one cupcake eater and the rest cupcake placers
  for(int i = 0; i < numberGuests; i++)
  {
    if(i == 0)
    {
      threads.emplace_back(guest, 1, i);
    } else
    {
      threads.emplace_back(guest, 0, i);
    }
    
  }

  //  wait for all threads to finish execution
  for(int i = 0; i < numberGuests; i++)
  {
    threads[i].join();
  }

  auto stop = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

  // calculate time in seconds
  double timeSeconds = duration.count() / 1000000.0;

  // Create output file and print output.
  ofstream cupcakesPrintFile("P1Result.txt");
  cupcakesPrintFile << "It took " << timeSeconds << " seconds and " << totalAccesses << " iterations for " << numberGuests << " threads to be sure everyone entered." << endl;
  cupcakesPrintFile.close();

  return 0;
}


