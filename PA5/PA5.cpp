/*
 * PA5.cpp
 *
 *  Created on: Feb 20, 2018
 *      Author: hjiang2
 */

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cstdio>
#include <algorithm>
#include "Ant.h"
#include "Doodlebug.h"
#include "Organism.h"
using namespace std;

/**
 * draw the board
 * @param &grid, the grid, we use 2 dimensional vector to represent the grid
 * @param gridSize, the size of the grid
 * @return void
 */
void drawBoard(vector<vector<Organism*> > &grid, int gridSize) {
	for (int i = 0; i < gridSize; i++) {
		for (int j = 0; j < gridSize; j++) {
			if(grid[i][j] == nullptr) {
				cout << ". ";
			}
			else {
				cout << grid[i][j]->getRep() << " ";
			}
		}
		cout << endl;
	}
	cout << endl;
}

/**
 * initialize the simulation
 * @param &grid, the grid, we use 2 dimensional vector to represent the grid
 * @param gridSize, the size of the grid
 * @param doodlebugs, the number of doodlebugs
 * @param ants, the number of ants
 * @return void
 */
void initializeSim(vector<vector<Organism*> > &grid, int gridSize, int doodlebugs, int ants){
	// initialize the board
	vector <pair<int, int> > all_pos;
	for (int y = 0; y < gridSize; y++){
		for (int x = 0; x < gridSize; x++){
			all_pos.push_back(make_pair(y, x));
		}
	}

	// get the position
	random_shuffle(all_pos.begin(), all_pos.end());

	// randomly assign positions for doodlebugs
	for (int i = 0; i < doodlebugs; i++){
		pair<int, int> pos = all_pos.back();
		all_pos.pop_back();
		new Doodlebug(grid, pos.second, pos.first);
	}

	// randomly assign positions for doodlebugs
	for (int i = 0; i < ants; i++){
		pair<int, int> pos = all_pos.back();
		all_pos.pop_back();
		new Ant(grid, pos.second, pos.first);
	}
}

/**
 * check the board to see if all Organisms are dead
 * @param &grid is the pointer to grid
 * @param gridSize is the size of the grid
 * @return boolean, true if all Organisms are dead
 */
bool allDead(vector<vector<Organism*> > &grid, int gridSize){
	for (int i = 0; i < gridSize; i++){
		for (int j = 0; j < gridSize; j++){
			if (grid[i][j] != nullptr){
				return false;
			}
		}
	}
	return true;
}

int main(int argc, char *argv[]){
	// initialize the variables and their defaults
	int gridSize = 20;
	int doodlebugs = 5;
	int ants = 100;
	int timeSteps = 1000;
	int seed = 1; // 0 means to use current time as the seed
	int pause = 0; // 0 means do not pause

	// command line rules:
	// if gridSize is specified
	if (argc >= 2){
		gridSize = atoi(argv[1]); // set gridSize to the specified #
	}
	// if doodlebugs is specified
	if (argc >= 3){
		doodlebugs = atoi(argv[2]); // set doodlebugs to the specified #
	}
	// if ants is specified
	if (argc >= 4){
		ants = atoi(argv[3]); // set ants to the specified #
	}
	// if timeSteps is specified
	if (argc >= 5){
		timeSteps = atoi(argv[4]); // set timeSteps to the specified #
	}
	// if seed is specified
	if (argc >= 6){
		seed = atoi(argv[5]); // set seed to the specified #
	}
	// if pause is specified
	if (argc >= 7){
		pause = atoi(argv[6]); // set pause to the specified #
	}
	// if too many arguments
	if (argc >= 8){
		cout << "Too many arguments!" << endl;
		return 0;
		// tell that there are too many arguments
	}
	// if seed is not set to 0(current time)
	if (seed != 0){
		srand(seed); // randomly generate a seed
	}
	// otherwise
	else{
		srand(time(NULL)); // randomly generate time
	}
	//create the grid with gridSize X gridSize of the size, every element is a pointer to a organism
	vector<vector<Organism*> > grid(gridSize, vector<Organism*>(gridSize, nullptr));

	//initialize the simulation (the grid)
	initializeSim(grid, gridSize, doodlebugs, ants);

	// simulation
	int step = 0;
	for (step = 1; step <= timeSteps; step++){
		// move doodlebugs first
		for (int i = 0; i < gridSize; i++){
			for (int j = 0; j < gridSize; j++){
				if (grid[i][j] != nullptr && grid[i][j]->getRep() == 'X' && grid[i][j]->movable){
					grid[i][j]->move();
				}
			}
		}

		// move ants after moving doodlebugs
		for (int i = 0; i < gridSize; i++){
			for (int j = 0; j < gridSize; j++){
				if (grid[i][j] != nullptr && grid[i][j]->getRep() == 'O' && grid[i][j]->movable){
					grid[i][j]->move();
				}
			}
		}

		// an Organism can move by default
		for (int i = 0; i < gridSize; i++){
			for (int j = 0; j < gridSize; j++){
				if (grid[i][j] != nullptr){
					grid[i][j]->movable = true;
				}
			}
		}

		// termination:
		// if # of steps specified on command is reached
		if (pause && step % pause == 0){
			drawBoard(grid, gridSize);
			// press any key to next
			cin.ignore();
		}
		// if all organisms are removed
		if (allDead(grid, gridSize)){
			break;
		}
	}

	// count the remaining organisms:
	// initialization
	int antRemain = 0;
	int doodlebugRemain = 0;
	// for everything in the board
	for (int i = 0; i < gridSize; i++){
		for (int j = 0; j < gridSize; j++){
			if (grid[i][j] != nullptr){
				// count the remaining ants
				if (grid[i][j]->getRep() == 'O'){
					antRemain++;
				}
				// count the remaining doodlebugs
				else{
					doodlebugRemain++;
				}
			}
		}
	}

	// print a summary
	cout << "Original command line: ";
	for (int i = 0; i < argc; i++){
		cout << argv[i] << " ";
	}
	cout << endl;

	cout << "Number of steps simulated: " << (step-1) << endl;


	cout << "The total number of ants during the course of the simulation is: "
			<< ants << ", and the number remaining is: " << antRemain << endl;

	cout << "The total number of doodlebugs during the course of the simulation is: "
			<< doodlebugs << ", and the number remaining is: " << doodlebugRemain << endl;

	// draw the board
	drawBoard(grid, gridSize); // ???

	// delete everything on the board (free from memory)
	for (int i = 0; i < gridSize; i++){
		for (int j = 0; j < gridSize; j++){
			if (grid[i][j] != nullptr){
				delete grid[i][j];
			}
		}
	}

	return 0;
}




