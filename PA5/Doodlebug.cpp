/*
 * Doodlebug.cpp
 *
 *  Created on: Feb 18, 2018
 *      Author: hjiang2
 */

#include "Doodlebug.h"
#include <cstdlib>
#include <iostream>
using namespace std;

Doodlebug::Doodlebug(vector<vector<Organism*> > &g, int x, int y): Organism(g, x, y), starvationSteps(0) {
	// TODO Auto-generated constructor stub

}

Doodlebug::~Doodlebug() {
	// TODO Auto-generated destructor stub

	grid[yPosition][xPosition] = nullptr;
}

void Doodlebug::move(){
	// initialization of variables
	int xNew = xPosition;
	int yNew = yPosition;
	int countEmpty = 0; // counts the possible directions to move to because of empty neighbors
	int directEmpty[4][2]; // assign a 2d array to indicate direction because of empty neighbors
	int countAnt = 0; // counts the possible directions to move to for eating ant
	int directAnt[4][2]; // assign a 2d array to indicate direction for eating ant

	// check if up direction is available to move
	// if Doodlebug is under the upper bound of the grid
	if(yPosition < gridSize - 1){
		// if the cell right above it is empty
		if(grid[yPosition + 1][xPosition] == nullptr){
			directEmpty[countEmpty][0] = yPosition + 1;
			directEmpty[countEmpty][1] = xPosition;
			countEmpty++;
		}
		// if the cell right above it has an ant
		else if(grid[yPosition + 1][xPosition]->getRep() == 'O'){
			directAnt[countAnt][0] = yPosition + 1;
			directAnt[countAnt][1] = xPosition;
			countAnt++;
		}
	}
	// if Doodlebug is above the lower bound of the grid
	if(yPosition > 0){
		// if the cell right below it is empty
		if(grid[yPosition - 1][xPosition] == nullptr){
			directEmpty[countEmpty][0] = yPosition - 1;
			directEmpty[countEmpty][1] = xPosition;
			countEmpty++;
		}
		// if the cell right below it has an ant
		else if(grid[yPosition - 1][xPosition]->getRep() == 'O'){
			directAnt[countAnt][0] = yPosition - 1;
			directAnt[countAnt][1] = xPosition;
			countAnt++;
		}
	}
	// if Doodlebug is on the right side of the left bound of the grid
	if(xPosition > 0){
		// if the cell on its left is empty
		if(grid[yPosition][xPosition - 1] == nullptr){
			directEmpty[countEmpty][0] = yPosition;
			directEmpty[countEmpty][1] = xPosition-1;
			countEmpty++;
		}
		// if the cell on its left has an ant
		else if(grid[yPosition][xPosition-1]->getRep() == 'O'){
			directAnt[countAnt][0] = yPosition;
			directAnt[countAnt][1] = xPosition-1;
			countAnt++;
		}
	}
	// if Doodlebug is on the left side of the right bound of the grid
	if(xPosition < gridSize-1){
		// if the cell on its right is empty
		if(grid[yPosition][xPosition + 1] == nullptr){
			directEmpty[countEmpty][0] = yPosition;
			directEmpty[countEmpty][1] = xPosition+1;
			countEmpty++;
		}
		// if the cell on its right has an ant
		else if(grid[yPosition][xPosition+1]->getRep() == 'O'){
			directAnt[countAnt][0] = yPosition;
			directAnt[countAnt][1] = xPosition+1;
			countAnt++;
		}
	}

	// what if there are ants and empty cells in the neighbor?
	// what if neighbor is full of other doodlebugs
	// if there are ants in the neighbor
	if(countAnt > 0){
		// choose a random number r that is in the range of ant count
		int r = rand() % countAnt;
		// set the the direction for where it moves for eating ant
		yNew = directAnt[r][0];
		xNew = directAnt[r][1];
		// the original position becomes empty
		grid[yPosition][xPosition] = nullptr;
		// change its x and y coordinates to the new position
		yPosition = yNew;
		xPosition = xNew;
		// delete the ant that originally occupies the cell
		// this happens automatically?
		delete grid[yPosition][xPosition];
		// update new position
		grid[yPosition][xPosition] = this;
		// its starvation stops in this step
		starvationSteps = 0;
	}
	else if(countEmpty>0){
		// choose a random number r that is in the range of empty cell count
		int r = rand() % countEmpty;
		// set the the direction for where it moves because of empty neighbor
		yNew = directEmpty[r][0];
		xNew = directEmpty[r][1];
		// the original position becomes empty
		grid[yPosition][xPosition] = nullptr;
		// change its x and y coordinates to the new position
		yPosition = yNew;
		xPosition = xNew;
		// update new position
		grid[yPosition][xPosition] = this;
		// its starvation increments in this step
		starvationSteps++;
	}

	// if it has been starving for 8 steps
	if(starvationSteps==8){
		delete this;
	}

	// surviving steps increases by 1
	surviveSteps++;
	// it can no longer move until the next step
	movable = false;


}

char Doodlebug::getRep(){
	return 'X';
}

void Doodlebug::breed(){
	int count = 0; // counts the possible directions to breed at
	int direct[4][2]; // assign a 2d array to indicate direction
	int breedSteps = surviveSteps; // counts the steps for calculating breed ability

	// if survives 3 steps
	if(breedSteps % 8 == 0){
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

			// assign x and y coordinations for the position of the new Doodlebug
			int yBreed = direct[r][0];
			int xBreed = direct[r][1];

			new Doodlebug(grid, xBreed, yBreed); // create a new Doodlebug with the new position
		}
		// if there is no empty neighbor available, it cannot breed
		else if(count<=0){
			breedSteps--; // decrement the breedSteps by 1 so it can still breed in the next step
		}
	}
}

