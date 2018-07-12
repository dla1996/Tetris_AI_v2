#include "Neuron.h"


Neuron::Neuron()
{
	neuronID = 0;
	_inputVal = 0.0;
	_outputVal = 0.0;
}

// Creating Neurons within loops
Neuron::Neuron(int numNeuron,int numConnections, bool isOutput, bool isInput)
{
	neuronID = numNeuron;
	_inputVal = 10.0;
	_outputVal = 0.0;
	_isInput = isInput;
	_isOutput = isOutput;

	//std::cout << "\tNeuron " << neuronID << " created" << std::endl;

	//// If this neuron is not the ouput layer
	//if (!isOutput)
	//{
	//	// Initially make inputs go to each output, including the bias beuron
	//	for (int i = 0; i < numConnections; i++)
	//	{
	//		Connection *newConnection = new Connection(i + 1);

	//		_connections.push_back(Connection(i + 1));
	//		_connectionTo.push_back(*newConnection);
	//	}
	//}
	

}

// Output neuron
Neuron::Neuron(int numNeuron, bool isOutput)
{
	neuronID = numNeuron;
	_inputVal = 10.0;
	_outputVal = 0.0;

	
}


// Used by other neurons
void Neuron::setInput(float inputVal)
{
	_inputVal = inputVal;
}

// Only used by input neurons, sets output at inputs
void Neuron::setInput(float inputVal, bool isInput)
{
	_inputVal = inputVal;
	_outputVal = _inputVal;
}


void Neuron::sigmoidFunction()
{
	_outputVal = 1 / (1 + exp(_inputVal));
}
