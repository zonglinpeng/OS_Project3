/*
 * Ant.h
 *
 *  Created on: Feb 18, 2018
 *      Author: hjiang2
 */

#ifndef ANT_H_
#define ANT_H_
#include "Organism.h"

class Ant: public Organism {
public:
	Ant(vector<vector<Organism*> > &g, int x, int y);
	virtual ~Ant();

	/**
	 * defines the function "move" outside the abstract class "Organism" for the drived class "Ant"
	 * randomly selects an unoccupied neighbor and moves to that neighbor
	 * doesn't move if all neighbors are occupied or off the grid
	 * @return void
	 */
	void move();

	/**
	 * if survives 3 steps, at the end of the 3rd step,
	 * gives birth to a new ant randomly on an empty neighbor
	 * if no empty neighbor available, no birth
	 * once gave birth, cannot give another until survives 3 additional steps
	 * @return void
	 */
	void breed();

	/**
	 * get the alphabet letter that represents ant
	 * @return char, 'O'
	 */
	char getRep();
};

#endif /* ANT_H_ */
