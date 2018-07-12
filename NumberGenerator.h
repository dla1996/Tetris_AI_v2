#pragma once

#ifndef __NUMBERGENERATOR_H
#define __NUMBERGENERATOR_H

#include <random>



class NumberGenerator
{
public:



private:
	std::random_device rd;
	std::mt19937 gen(rd());


};

#endif