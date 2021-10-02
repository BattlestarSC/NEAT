#pragma once
#include <vector>
#include "node.h"
#include "connection.h"
class individual
{
public:
	double fitness{ 0.0 };
	std::vector<node*> nodes;
	int inputSize;
	int outputSize;

	individual(individual* cpy, bool mutate = false);
	individual(individual* father, individual* mother, bool mutate = true);

};

