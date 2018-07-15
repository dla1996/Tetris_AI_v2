#pragma once
#ifndef __NEURON_H
#define __NEURON_H
#include <iostream>
#include <vector>
#include <math.h>

#include "Connection.h"

class Connection;
#define INPUT_COUNT 6	/*
							6 Inputs:
							Score
							# Holes in first layer
							Weighted height
							Highest relative column
							Cumulative height
							# of lines cleared
							*/

#define OUTPUT_COUNT 4	// Output controls( up, down, left, right )

class Neuron
{
public:
	Neuron();
	Neuron(int numNeuron);
	Neuron(int numNeuron, bool isOutput);
	Neuron(int numNeuron, int numConnections, bool isOutput, bool isInput);

	// Sets the input values based on the dot product of all input weights and values
	void setInput(float inputVal);

	void setInput(float inputVal, bool isInput);

	// Sets the output value based on sigmoid Function
	void sigmoidFunction();
	

	// Identifier for neuron
	int neuronID;

	// Sum of inputs & weights
	float _inputVal;
	float _outputVal = -10.0;	// bias


	// Connections with weights for each next connected neuron
	std::vector<Connection> _connections;
	std::vector<Connection> _connectionFrom;
	std::vector<Connection> _connectionTo;
private:
	bool _isInput = false;
	bool _isOutput = false;

};


#endif