#include "Connection.h"

Connection::Connection(int connectionNum)
{
	// Generate weight between -1 to +1
	//weight = -1 + (float)(rand() / (RAND_MAX / (1 - (-1))));
	// Generate weight between 0 to 1
	weight = ((double) rand() / RAND_MAX);


	connectID = connectionNum;
	//std::cout << "\t\tConnection " << connectID << " created" << std::endl;
}

void Connection::setValues(float inputVal)

{
	_inputVal = inputVal;
	_passVal = _inputVal * weight;
}