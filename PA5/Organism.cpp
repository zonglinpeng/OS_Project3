/*
 * Organism.cpp
 *
 *  Created on: Feb 18, 2018
 *      Author: hjiang2
 */

#include "Organism.h"

Organism::Organism(vector<vector<Organism*> > &g, int x, int y):
		movable(true), surviveSteps(0), xPosition(x), yPosition(y), grid(g), gridSize(g.size())
{
// TODO Auto-generated constructor stub
// find in the grid
grid[y][x] = this;
}


