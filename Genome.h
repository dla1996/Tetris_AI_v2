#pragma once

#ifndef __GENOME_H
#endif	__GENOME_H

#include <iostream>
#include "Connection.h"
#include "NET.h"
#include "Neuron.h"


const float MutateConnectionsChance = 0.25;
const float LinkMutationChance = 2.0;
const float NodeMutationChance = 0.5;
const float BiasMutationChance = 0.4;
const float DisableMutationChance = 0.4;
const float EnableMutationChance = 0.2;

class Mutations
{
public:
	Mutations(std::string nameOf, float mutationRate)
	{
		typeMutation = nameOf;
		_mutationRate = mutationRate;
	}
	std::string typeMutation;
	float _mutationRate;
};

class Genome
{
public:

	Genome();

	// Function to keep updating the fitness and input values
	void setValues(float _score, float _insertRowsCleared,
		float _insertWeightedHeight,
		float _insertCumulativeHeight,
		float _insertRelativeHeight,
		float _insertHoles,
		float _insertRoughtness);

	/*void updateFitness(float _score, float _insertRowsCleared,
		float _insertWeightedHeight,
		float _insertCumulativeHeight,
		float _insertRelativeHeight,
		float _insertHoles,
		float _insertRoughtness);*/

	void updateFitness(float currFitness);


	int getID()
	{
		return id;
	}

	std::vector<float> getDecision()
	{
		return moves;
	}


	std::vector<Mutations> mutationRates;

	// Initially create only input and output layers
	NET *thisGenomeNet = new NET(6, 4);
	//NET *thisGenomeNet = new NET(6, 8, 4);

private:

	int id;

	float score;

	float rowsCleared;
	// The abosule height of the highest column to the power of 1.5
	// Added so that the algorithm can be able to detect if the blocks are stacking too high
	float weightedHeight;
	// the sum of all the column's heights
	float cumulativeHeight;
	// The highest column minus the lowest columns
	float relativeHeight;
	// The sum of all the empty cells that have a block
	// above them
	float holes;
	// The sum of absolute differences between the height of each columns
	float roughness;

	float fitness;

	
	

	// Vector values to determine what move to make
	std::vector<float> moves;
};
