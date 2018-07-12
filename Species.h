#pragma once
#ifndef __SPECIES_H
#define __SPECIES_H

#include "Genome.h"

class Species
{
public:

	std::vector<Genome> genomes;
	float topFitness = 0.0;
	float staleness = 0.0;
	float averageFitness = 0.0;

private:

};

#endif	