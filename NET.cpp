#include "NET.h"

const float connectionChance = 0.3; // Chance to connect nodes
const float nodeChance = 0.1; // Chance to create node/neuron
const float hiddenLayerBias = 0.5;

// Default constructor to create the input layer and output layers
NET::NET(int inputs, int outputs)
{

	_numInputs = inputs;
	_numOutputs = outputs;

	 // Random float value to see if connections are made
	 // 0.0 - 1.0
	 float n = ((double)rand() / RAND_MAX);

	 // Random float value to see if node will be made
	 float x = ((double)rand() / RAND_MAX);

	 // Random value for which input to connect from
	 float c = rand() % inputs;

	 // Random value for which input to connect to
	 float t = rand() % outputs;

	


	//std::cout << "Net being made" << std::endl;
	m_Layers.push_back(Layer());
	//std::cout << "Input" << std::endl;
	for (int i = 0; i <= inputs; i++)
	{
		// Make each new input neuron, plus a bias
		m_Layers.back().push_back(Neuron(i + 1, 0, false, true));
	}

	m_Layers.push_back(Layer());

	//std::cout << "Output" << std::endl;
	for (int i = 0; i < outputs; i++)
	{
		// Make each new output neuron
		m_Layers.back().push_back(Neuron(i + 1, 0, true, false));

	}

	// 30% chance of creating a connection between a random input neuron and output neuron
	if (n < connectionChance)
	{
		makeConnection(m_Layers[0][c], m_Layers[m_Layers.size() - 1][t]);
	}

	// Try to add new neurons between each input and output
	/*if(x < nodeChance)
	{
		addNeuron(m_Layers[0][i], m_Layers[1][t]);
	}*/



	
}


/*
	This can be deleted...
*/
NET::NET(int inputs, int hiddenNodes, int outputs)
{
	_numInputs = inputs;
	_numHidden = hiddenNodes;
	_numOutputs = outputs;

	std::cout << "Created Input Layer" << std::endl;
	m_Layers.push_back(Layer());
	for (int i = 0; i <= inputs; i++)
	{
		// Make each new input neuron, plus a bias
		m_Layers.back().push_back(Neuron(i + 1, hiddenNodes, false, true));
	}

	std::cout << "Created Hidden Layer" << std::endl;
	m_Layers.push_back(Layer());
	for (int i = 0; i <= hiddenNodes; i++)
	{
		// Make each new input neuron, plus a bias
		m_Layers.back().push_back(Neuron(i + 1, outputs, false, false));
	}

	std::cout << "Created Output Layer" << std::endl;
	m_Layers.push_back(Layer());
	for (int i = 0; i < outputs; i++)
	{
		// Make each new input neuron, no bias of course
		m_Layers.back().push_back(Neuron(i + 1, true));
	}

	std::cout << "Connecting inputs to hidden" << std::endl;
	// Create connections...
	// Including bias...
	for (int i = 0; i < inputs + 1; i++)
	{
		// Go to first layer, input, go to each neuron
		// Make a connection
		for (int j = 0; j < hiddenNodes; j++)
		{
			std::cout << "\t\t\tConnecting Input Neuron " << m_Layers[0][i].neuronID <<
			" to hidden Neuron" << m_Layers[1][j].neuronID << std::endl;
			m_Layers[0][i]._connections[j].from = &m_Layers[0][i];	// each connection comes from this layer's neuron
			m_Layers[0][i]._connections[j].to = &m_Layers[1][j];	// Each connection goes to next layer's neuron

		}
	}

	std::cout << "Connecting hidden to output" << std::endl;

	// Create connections...
	// Including bias...
	for (int i = 0; i < hiddenNodes + 1; i++)
	{
		// Go to first layer, input, go to each neuron
		// Make a connection
		for (int j = 0; j < outputs; j++)
		{
			std::cout << "\t\t\tConnecting hidden Neuron " << m_Layers[1][i].neuronID <<
			" to Output Neuron" << m_Layers[2][j].neuronID << std::endl;
			m_Layers[1][i]._connections[j].from = &m_Layers[1][i];	// each connection comes from this layer's neuron
			m_Layers[1][i]._connections[j].to = &m_Layers[2][j];	// Each connection goes to next layer's neuron
			checkConnection(m_Layers[1][i], m_Layers[2][j]);
		}
	}



}

// 4 output vector, 
std::vector<float> NET::evaluateInputs()
{
	std::vector<float> output;

	// Go through each input neuron...
	for (int i = 0; i < _numInputs; i++)
	{
		// If there are connections at input neuron...
		if (m_Layers[0][i]._connections.size() != 0)
		{
			//std::cout << "Connection at  input neuron " << i;
			// Go through each connection available at this neuron...
			for (int k = 0; k < m_Layers[0][i]._connections.size(); k++)
			{
				// Set each connection within each neuron in the input layer, with the value from the neuron's output.
				m_Layers[0][i]._connections[k].setValues(m_Layers[0][i]._inputVal);
				m_Layers[0][i]._connectionTo[k].setValues(m_Layers[0][i]._inputVal);

			}
			//// Propagate inputs through connections passVal
			//// Even bias node
			//for (int i = 0; i <= _numInputs; i++)
			//{
			//	for (int j = 0; j < _numHidden; j++)
			//	{
			//		// Set each connection within each neuron in the input layer, with the value from the neuron's output.
			//		m_Layers[0][i]._connections[j].setValues(m_Layers[0][i]._inputVal);
			//	}
			//}
		}
	}


	// If there are some hidden layers...
	if(_numHidden != 0)
	{
		// For each hidden layer's nodes
		// Calculate sum of all input layer nodes
		for (int i = 0; i < _numHidden; i++)
		{
			float sumForNode = 0.0;
			for (int j = 0; j <= _numInputs; j++)
			{
				sumForNode += m_Layers[0][j]._connections[i]._passVal;
			}

			// Each node in input layer, points to the same node, so can do this
			// To node, set the Input value for that node
			m_Layers[0][0]._connections[i].to->setInput(sumForNode);
		}

		// Then do sigmoid function
		// For each hidden neuron, after getting inputs, calculate outputvalue with sigmoidFunction
		for (int i = 0; i < _numHidden; i++)
		{

			m_Layers[0][0]._connections[i].to->sigmoidFunction();
		}


		// Propagate 
		for (int i = 0; i < _numOutputs; i++)
		{
			float sumForNode = 0.0;
			for (int j = 0; j < _numHidden; j++)
			{
				sumForNode += m_Layers[1][j]._connections[i]._passVal;
			}

			m_Layers[1][0]._connections[i].to->setInput(sumForNode);

		}
	}
	else
	{
		// If there are no hidden neurons...
		// Go through each output to perform sigmoid
		for (int i = 0; i < _numOutputs; i++)
		{
			// Check each output neuron for any connection to it
			if (m_Layers[m_Layers.size() - 1][i]._connectionFrom.size() != 0)
			{
				float sumForNode = 0.0;
				// If it has connections...
				// Go through each connection
				for (int k = 0; k < m_Layers[m_Layers.size() - 1][i]._connectionFrom.size(); k++)
				{
					// Take the sum of each connection
					sumForNode += m_Layers[m_Layers.size() - 1][i]._connectionFrom[k]._passVal;
				}

				// Then at this output neuron...
				// Output layer's input = to sum of each connection pass value
				m_Layers[m_Layers.size() - 1][i]._connectionFrom[0].to->setInput(sumForNode);

				// Then at the same neuron..
				// Perform the sigmoid function
				m_Layers[m_Layers.size() - 1][i]._connectionFrom[0].to->sigmoidFunction();
			}
		}

		
	}



	return output;
}

// Add a neuron in between two neurons
void NET::addNeuron(Neuron &n1, Neuron &n2)
{
	Neuron *neuron1 = &n1, *neuron2 = &n2;
	Neuron *newNeuron = new Neuron();
	int connection = checkConnection(n1, n2);
	// If there is a connection present...
	if (connection)
	{
		// Connect 
		Connection *newConnector;
		// Repoint old connection from to point to new neuron
		neuron1->_connections[connection - 1].from = newNeuron;

		// newNeuron takes neuron1's old connection
		newNeuron->_connections.push_back(neuron1->_connections[connection - 1]);

		// Very confusing but, after push_back, make newNeuron's new connection at current size - 1,
		// its connectionID equal to the current vector size.
		newNeuron->_connections[newNeuron->_connections.size() - 1].connectID = newNeuron->_connections.size();

		// Neuron1's new connector now takes neuron1's old connection's ID
		newConnector->connectID = neuron1->_connections[connection - 1].connectID;
		newConnector->from = neuron1;
		newConnector->to = newNeuron;

		// Replace neuron's connection with the new one pointint to new neuron
		neuron1->_connections[connection - 1] = *newConnector;
		
		// newConnector values, input and output
		newConnector->setValues(neuron1->_outputVal);

	}

}


// Make connection from n1 to n2
void NET::makeConnection(Neuron &n1, Neuron &n2)
{
	Neuron *neuron1 = &n1;
	Neuron *neuron2 = &n2;

	int numConnections = neuron1->_connections.size();
	Connection *connector = new Connection(numConnections+1);

	int connection = checkConnection(n1, n2);

	// If no connection present...
	if (connection == 0)
	{
		std::cout << "Making connection between " << n1.neuronID - 1  << " and " << n2.neuronID - 1 << std::endl;

		// Make connector point from n1 to n2
		connector->from = neuron1;
		connector->to = neuron2;

		// Make connector pass n1 value with weight for later
		connector->setValues(neuron1->_outputVal);

		// Push this new object to neuron to from list
		neuron1->_connections.push_back(*connector);
		neuron1->_connectionTo.push_back(*connector);
		neuron2->_connectionFrom.push_back(*connector);
	}



}


// Check if there is a connection between n1 and n2
int NET::checkConnection(Neuron &n1, Neuron &n2)
{
	int connectionID = 0;
	// Iterate through n1's connections
	std::vector<Connection>::iterator it;
	for (it = n1._connections.begin(); it != n1._connections.end(); it++)
	{
		// Check if the connector is coming from and to the same addresses as n1 and n2
		if (it->from == &n1 && it->to == &n2)
		{
			std::cout << "Connection present between " << n1.neuronID - 1 << " and " << n2.neuronID - 1 << std::endl;
			
			return it->connectID;
		}
	}

	std::cout << "No connection between " << n1.neuronID - 1 << " and " << n2.neuronID - 1 << std::endl;
	return 0;

}

void NET::setInputValues(float score, float rowsCleared, float weightedHeight, float cumulativeHeight, float numHoles, float valRoughness)
{

	// Set the Input layer Neurons accordingly
	for (int i = 0; i < INPUT_COUNT; i++)
	{
		switch (i)
		{
			case 0:
				m_Layers[0][i].setInput(score, true);
					break;
			case 1:
				m_Layers[0][i].setInput(rowsCleared, true);
				break;
			case 2:
				m_Layers[0][i].setInput(weightedHeight, true);
				break;
			case 3:
				m_Layers[0][i].setInput(cumulativeHeight, true);
				break;
			case 4:
				m_Layers[0][i].setInput(numHoles, true);
				break;
			case 5:
				m_Layers[0][i].setInput(valRoughness, true);
				break;

			default:
				std::cout << "Invalid Value" << std::endl;
				break;
		}
		
	}
}


