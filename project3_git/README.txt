README
Author: Zonglin Peng, Rui Huang
Date: 2/15/2019

=====================================Part 1=================================================
# DESCRIPTION

In this question, we implemented a queue that record the arrival time of the actors, 
then use several semaphore to allow only desired actors to get in.

Our streategy is:
 - who comes first at the beginning claim the "turn" = (PIRATE/NINJA).
 - for any actor(threads), they can enter the building if:
   1. there is no opponent in the building
   2. there is available spot in the building
   3. either:
     (a) ally hold the "turn" and thread is the first in queue amongst allies, OR
     (b) the thread is the first in entire queue
 - Turn: will be swapped if N conseqtive ninjia/pirate goes into the shop where 
	N is the number of costume team.

Once they satisfies the requirement, they can get in the building. And our testing 
results shows that this streategy can effectively reduce the starving time and allow 
more concurrency.

NOTE: the one long query is preserved with complete fairness and mystery.


=====================================Part 2=================================================
Instruction:
1. cd to part2, and make file
2. run part2 executable, read print message about cars
3. make clean

Designs and Explanations:
Besides the design details elabrated in problem2_explation/Our_Design, this part uses pthread_mutex. Each car is a thread and all information including ID, Direction, Destination, and Path are randomly generated, where Path and Destination is calculated numerically from its Destination. Global datas are the four queues for each lane which is protected by their own mutex, the limit of car that are in the intersection and protected by its mutex. Four sections of the intersection is protected by an array of four mutexs. Only the first car of a queue has the chance to try to enter the intersection, and it enters only if the limit is 3 if all car perform left/straight, or 4 if one or more car is doing right turn, and the next section of the car is available. Fairness is given to each of the head car of four queues, but depends on how fast the car in the intersection moves around.

Testing:
Test the program simply by running the executale without input command. An example of a whole run is shown in the test_phast2.txt, where the message "MA DRIVERS ALL SURVUVED!!!" at the end indicates the program has finished and terminated succesfully. In the current version, each car has a 80% chance to come back by arriving at a random queue with random new information. In our test, all cars will be reused (infinite looping) and no deadlock was encountered after running for 20 minutes. More details can be referred in the print messages.
