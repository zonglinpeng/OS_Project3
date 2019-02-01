/*
 * Doodlebug.h
 *
 *  Created on: Feb 18, 2018
 *      Author: hjiang2
 */

#ifndef DOODLEBUG_H_
#define DOODLEBUG_H_
#include "Organism.h"

class Doodlebug: public Organism {
public:
	Doodlebug(vector<vector<Organism*> > &g, int x, int y);
	virtual ~Doodlebug();

	/**
	 * defines the function "move" outside the abstract class "Organism" for the drived class "Doodlebug"
	 * if there are ants in the neighbors, it randomly chooses one, moves to it and eats the ant
	 * if no ants in the neighbors, it moves to a random empty cell
	 * cannot eat another doodlebug
	 * if neighbors are occupied with other doodlebugs, it doesn't move
	 * @return void
	 */
	void move();

	/**
	 * if survives 8 steps, at the end of the 8th step,
	 * gives birth to a new doodlebug randomly on an empty neighbor
	 * if no empty neighbor available, no birth
	 * once gave birth, cannot give another until survives 8 additional steps
	 * starvation goes ahead of breed
	 * @return void
	 */
	void breed();

	/**
	 * get the alphabet letter that represents doodlebug
	 * @return char, 'X'
	 */
	char getRep();

private:
	int starvationSteps; // counts the steps for calculating starvation occurence
};

#endif /* DOODLEBUG_H_ */
