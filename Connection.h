#pragma once
#ifndef __CONNECTION_H
#define __CONNECTION_H

#include <iostream>
#include "Neuron.h"

class Neuron;

// Connecting from neuron to neuron
class Connection
{
public:
	Connection(int connectionNum);

	void setValues(float inputVal);

	// Connection from a neuron
	Neuron *from = NULL;

	// Connection to a neuron
	Neuron *to = NULL;

	// Connection identifier
	int connectID;

	// Weight value of THIS connection
	float weight;

	//	value that needs to be passed
	float _inputVal;

	// Multiplication of weight * inputVal that is passed to next neuron
	float _passVal;

private:
};

#endif