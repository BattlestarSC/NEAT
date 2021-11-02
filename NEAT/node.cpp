#include "pch.h"
#include "node.h"
#include <algorithm>

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

bool connectionCompare(connection* a, connection* b)
{
	// <= rather than < so that in the rare case where some stupid error caused duplicates, no infinate loop is created and order is maintained
	return (a->innovation <= b->innovation);
}

/*
* I would like to make this a Radix sort
* 2 reasons: 
*		1) Its cool
*		2) At massive network sizes, it might actually be faster
*/
void node::sort()
{
	// Sort both vectors
	std::sort(this->forward.begin(), this->forward.end(), connectionCompare);
	std::sort(this->memory.begin(), this->memory.end(), connectionCompare);
}

// Initial constructor
node::node(std::vector<connection*> connections, unsigned long long int ID)
{
	id = ID;
	if (!connections.empty()) {
		for (connection* c : connections) {
			forward.push_back(new connection(c));
		}
	}
	// explicitly create an empty vector for my peace of mind, unnecessary
	this->memory = std::vector<connection*>();
}

node::node(std::vector<connection*> connections, std::vector<connection*> mem, unsigned long long int ID)
{
	id = ID;
	if (!connections.empty()) {
		for (connection* c : connections) {
			forward.push_back(new connection(c));
		}
	}
	if (!memory.empty()) {
		for (connection* c : memory) {
			forward.push_back(new connection(c));
		}
	}
}

// Copy constructor
node::node(node* cpy)
{
	id = cpy->id;
	if (!cpy->forward.empty()) {
		for (connection* c : cpy->forward) {
			forward.push_back(new connection(c));
		}
	}
	if (!cpy->memory.empty()) {
		for (connection* c : cpy->memory) {
			memory.push_back(new connection(c));
		}
	}
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

double node::activation(double v)
{
	// The NEAT paper uses a steepened sigmoid to speed training
	// I'm going to implment a default smoothed sigmoid instead to allow for greater "finesse"
	// 1 / 1 + e^(-0.125X)
	// Not sure if it will work that way or not, but worth a try
	// This is a "intuitive" guess
	double sigmoid = v * -0.125;
	sigmoid = exp(sigmoid);
	sigmoid += 1.0;
	sigmoid = 1.0 / sigmoid;
	return sigmoid;
}
