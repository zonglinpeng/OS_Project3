/*
 * Ant.cpp
 *
 *  Created on: Feb 18, 2018
 *      Author: hjiang2
 */

#include "Ant.h"
#include <vector>
#include <cstdlib>
using namespace std;

Ant::Ant(vector<vector<Organism*> > &g, int x, int y): Organism(g, x, y) {
	// TODO Auto-generated constructor stub

}

/**
 * defines the function "move" outside the abstract class "Organism" for the drived class "Ant"
 * randomly selects an unoccupied neighbor and moves to that neighbor
 * doesn't move if all neighbors are occupied or off the grid
 * @return void
 */
void Ant::move(){

	// initialization of variables
	int xNew = xPosition;
	int yNew = yPosition;
	int count = 0; // counts the possible directions to move to
	int direct[4][2]; // assign a 2d array to indicate direction

	// check if up direction is available to move
	// if Ant is under the upper bound of the grid and the cell right above it is empty
	if(yPosition < gridSize - 1 && grid[yPosition + 1][xPosition] == nullptr){
		// set the direction to "up"
		direct[count][0] = yPosition + 1;
		direct[count][1] = xPosition;
		// increment "count" by 1
		count++;
	}

	// check if down direction is available to move
	// if Ant is above the lower bound of the grid and the cell right below it is empty
	if(yPosition > 0 && grid[yPosition - 1][xPosition] == nullptr){
		// set the direction to "down"
		direct[count][0] = yPosition - 1;
		direct[count][1] = xPosition;
		// increment "count" by 1
		count++;
	}

	// check if left direction is available to move
	// if Ant is on the right side the left bound of the grid and the cell on its left it is empty
	if(xPosition > 0 && grid[yPosition][xPosition-1] == nullptr){
		// set the direction to "left"
		direct[count][0] = yPosition;
		direct[count][1] = xPosition-1;
		// increment "count" by 1
		count++;
	}

	// check if right direction is available to move
	// if Ant is on the left side the right bound of the grid and the cell on its right it is empty
	if(xPosition < gridSize - 1 && grid[yPosition][xPosition+1] == nullptr){
		// set the direction to "left"
		direct[count][0] = yPosition;
		direct[count][1] = xPosition+1;
		// increment "count" by 1
		count++;
	}

	// if there are more than 1 possible directions to move to
	if(count > 0){

		// choose a random number r that is in the range of count
		int r = rand() % count;

		// set the the direction for where it moves
		yNew = direct[r][0];
		xNew = direct[r][1];

		// the original position becomes empty
		grid[yPosition][xPosition] = nullptr;

		// change its x and y coordinates to the new position
		yPosition = yNew;
		xPosition = xNew;

		// update new position
		grid[yPosition][xPosition] = this;

		// it can possibly move and breed
		breed();
	}

	// surviving steps increases by 1
	surviveSteps++;

	// it can no longer move until the next step
	movable = false;
}

/**
 * if survives 3 steps, at the end of the 3rd step,
 * gives birth to a new ant randomly on an empty neighbor
 * if no empty neighbor available, no birth
 * once gave birth, cannot give another until survives 3 additional steps
 * @return void
 */
void Ant::breed(){
	int count = 0; // counts the possible directions to breed at
	int direct[4][2]; // assign a 2d array to indicate direction
	int breedSteps = surviveSteps; // counts the steps for calculating breed ability

	// if survives 3 steps
	if(breedSteps % 3 == 0){
		// up
		if(yPosition < gridSize - 1 && grid[yPosition + 1][xPosition] == nullptr){
			// set the direction to "up"
			direct[count][0] = yPosition + 1;
			direct[count][1] = xPosition;
			// increment "count" by 1
			count++;
		}
		// down
		if(yPosition > 0 && grid[yPosition - 1][xPosition] == nullptr){
			// set the direction to "down"
			direct[count][0] = yPosition - 1;
			direct[count][1] = xPosition;
			// increment "count" by 1
			count++;
		}
		// left
		if(xPosition > 0 && grid[yPosition][xPosition-1] == nullptr){
			// set the direction to "left"
			direct[count][0] = yPosition;
			direct[count][1] = xPosition-1;
			// increment "count" by 1
			count++;
		}
		// right
		if(xPosition < gridSize - 1 && grid[yPosition][xPosition+1] == nullptr){
			// set the direction to "left"
			direct[count][0] = yPosition;
			direct[count][1] = xPosition+1;
			// increment "count" by 1
			count++;
		}

		// if it can breed
		if(count>0){
			int r = rand() % count; // choose a random number r that is in the range of count

			// assign x and y coordinations for the position of the new Ant
			int yBreed = direct[r][0];
			int xBreed = direct[r][1];

			new Ant(grid, xBreed, yBreed); // create a new Ant with the new position
		}
		// if there is no empty neighbor available, it cannot breed
		else if(count<=0){
			breedSteps--; // decrement the breedSteps by 1 so it can still breed in the next step
		}
	}


}



Ant::~Ant() {
	// TODO Auto-generated destructor stub

	// delete from the grid
	grid[yPosition][xPosition] = nullptr;
}

/**
 * get the alphabet letter that represents the Organism
 * @return char, 'O'
 */
char Ant::getRep(){
	return 'O';
}

