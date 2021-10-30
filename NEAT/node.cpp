#include "pch.h"
#include "node.h"

#include <vector>
#include <cmath>

/*
* Add to the tallyed input during a run
*/
void node::addInput(connection* in, double value)
{
	this->output += in->weight * value;
}

/*
* Get the output of the node, either for execution of a future node or for a memory conneciton
* memory connections access the past value
* output or forward connections access the current one 
*/
double node::getOutput(bool past)
{
	if (past) {
		return previous;
	}

	return activation(output);
}

/*
* This is for in between each network execution
*/
void node::reset()
{
	previous = activation(output);
	output = 0;
}

node::node(std::vector<connection*> connections)
{
}

node::node(std::vector<connection*> connections, std::vector<connection*> mem)
{
}

node::node(node* cpy)
{
}

/*
* Free memory on destruction
*/
node::~node()
{
	for (connection* c : this->forward) {
		delete c;
	}
	for (connection* c : this->memory) {
		delete c;
	}
}

bool node::connectionCompare(connection* a, connection* b)
{
	// <= rather than < so that in the rare case where some stupid error caused duplicates, no infinate loop is created and order is maintained
	return (a->innovation <= b->innovation);
}

double node::activation(double v)
{
	// The NEAT paper uses a steepened sigmoid to speed training
	// I'm going to implment a default smoothed sigmoid instead to allow for greater "finesse"
	// 1 / 1 + e^(-0.125X)
	// Not sure if it will work that way or not, but worth a try
	double sigmoid = v * -0.125;
	sigmoid = exp(sigmoid);
	sigmoid += 1.0;
	sigmoid = 1.0 / sigmoid;
	return sigmoid;
}
