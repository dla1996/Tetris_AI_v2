#pragma once
#ifndef __POOL_H
#define __POOL_H
#include <iostream>
#include <vector>
#include "Species.h"

#define OUTPUTS 4

class Pool
{
public:

	std::vector<Species> species;
	int generation = 0;
	int innovation = OUTPUTS;
	int currentSpecies = 1;
	int currentGenome = 1;
	int maxFitness = 0;
	
private:

};


#endif