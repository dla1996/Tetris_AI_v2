#include "Genome.h"


Genome::Genome()
{
	// SUCCESS - std::cout << "Genome created" << std::endl;
	// Unique identifier for a genome
	id = rand();

	std::cout << "Genome: " << id << " created" << std::endl << std::endl;

	mutationRates.push_back(Mutations("connections", MutateConnectionsChance));
	mutationRates.push_back(Mutations("Link", LinkMutationChance));
	mutationRates.push_back(Mutations("bias", BiasMutationChance));
	mutationRates.push_back(Mutations("Node", NodeMutationChance));
	mutationRates.push_back(Mutations("enable", EnableMutationChance));
	mutationRates.push_back(Mutations("disable", DisableMutationChance));

	fitness = 0.0;


}

// Function to keep updating the fitness and input values
void Genome::setValues(float _score, float _insertRowsCleared,
	float _insertWeightedHeight,
	float _insertCumulativeHeight,
	float _insertRelativeHeight,
	float _insertHoles,
	float _insertRoughtness)
{
	score = _score;
	rowsCleared = _insertRowsCleared;
	weightedHeight = pow(_insertWeightedHeight, 1.5);
	relativeHeight = _insertRelativeHeight;
	cumulativeHeight = _insertCumulativeHeight;
	holes = _insertHoles;
	roughness = _insertRoughtness;

	thisGenomeNet->setInputValues(score, rowsCleared, weightedHeight, cumulativeHeight, holes, roughness);
	moves = thisGenomeNet->evaluateInputs();
}

//void Genome::updateFitness(float _score, float _insertRowsCleared,
//	float _insertWeightedHeight,
//	float _insertCumulativeHeight,
//	float _insertRelativeHeight,
//	float _insertHoles,
//	float _insertRoughtness)
//{
//	fitness = 2 * _score + 5 * _insertRowsCleared - 1.5 * _insertWeightedHeight - _insertCumulativeHeight - _insertRelativeHeight - _insertHoles - _insertRoughtness;
//}

void Genome::updateFitness(float currFitness)
{
	fitness = currFitness;
}