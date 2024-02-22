// Mathew Reilly
// COP4520
// Assignment 2
// Description:
//    .
#include <thred>
#include <mutex>
#include <chrono>
using namespace std;


/*
Problem 1:
The guests may enter the labyrinth, one at a time, and only when he invites them to do so.

When a guest reaches the end of the labyrinth, they may decide to eat the cupcake or leave it.

If a cupcake was eaten by a previous guest, the plate will be empty when the next guest arrives, and
they may requst for a cupcake.

When the new cupcake is brought, that guest may eat it or leave it on the plate.

Withought allowing the guest to talk to eachother once the game has started, how can they let the Mintaur know they all
entered the labyrinth.

Simulate the winning protocol.

*/


/*
Problem 2:
Pick one of the 3 strategies/protocol to enter a showroom one at a time.

1) Guest can check to see if the showroom door is open and try to enter the room.

2) Allows guest to flip a sign at the door indicating "busy" or "available". guest must set the showroom to busy when entering
available while leaving.

3) Guest would line in a queue. Every guest exiting the room is responsible for notifying guest standing in front of the queue
that the showroom is available. Guest can queue multiple times.

Which strategy will you choose and why? - what are the advantages and disadvantages

*/

int main()
{

  return 0;
}