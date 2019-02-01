/*
 * Organism.h
 *
 *  Created on: Feb 18, 2018
 *      Author: hjiang2
 */

#ifndef ORGANISM_H_
#define ORGANISM_H_
#include <vector>
using namespace std;

class Organism {
public:
	Organism(vector< vector<Organism*> > &g, int x, int y);
	virtual ~Organism() {};

	// the virtual function "move"
	// defined in derived classes
	virtual void move() = 0;

	/**
	 * the virtual function "getRep"
	 * defined in derived classes
	 * get the alphabet letter that represents the Organism
	 * @return 'O' if Organism is an Ant, 'X' for Doodlebug
	 */
	virtual char getRep() = 0;

	/**
	 * indicate whether if an Organism can move
	 * @return true if it can move
	 */
	bool movable;

protected:
	// basic data common to Ant and Doodlebug

	// the number of steps it servives
	int surviveSteps;

	// the x and y coordinates
	int xPosition;
	int yPosition;

	// the grid
	// we use 2 dimensional vector to represent the grid
	// because it is easy to access by indexes in the x and y directions
	vector< vector<Organism*> > &grid;

	// the size of the grid
	int gridSize;
};

#endif /* ORGANISM_H_ */
