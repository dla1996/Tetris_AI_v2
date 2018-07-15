#pragma once


#ifndef __NET_H
#define __NET_H

#include <iostream>
#include <vector>
#include "Neuron.h"

// Each genome has their own Net
// Initially should start as bare bones ( input goes to output directly )
class NET
{
public:

	NET(int inputs, int outputs);
	NET(int inputs, int hiddenNodes, int outputs);
	// Returns which connectionID is present between neurons
	int checkConnection(Neuron & n1, Neuron & n2);
	void setInputValues(float score, float rowsCleared, float weightedHeight, float cumulativeHeight, float numHoles, float valRoughness);
	void makeConnection(Neuron &n1, Neuron &n2);

	


	// Get a 4 vector output from current inputs.
	std::vector<float> evaluateInputs();
	void addNeuron();
	void addNeuron(Neuron & n1, Neuron & n2);

	// Layer of neurons
	typedef std::vector<Neuron> Layer;

	int _numInputs = 0;
	int _numHidden = 0;
	int _numOutputs = 0;
	// Consists of 1 layer of input
	// Then hidden layers
	// Then output
	std::vector<Layer> m_Layers;


};


#endif