#include "pch.h"
#include "node.h"
#include <cmath>

double node::activation()
{
	//just use the steepend sigmoid
	double d = this->sense * -4.9;
	d = exp(d) + 1.0;
	d = 1.0 / d;
	return d;
}

node::node(node* cpy)
{
	this->ID = cpy->ID;
	for (auto* c : cpy->connections) {
		this->connections.push_back(new connection(c));
	}
}

void node::addConnection(connection* add)
{
	this->connections.push_back(add);
}

double node::getOutput()
{
	return this->activation();
}

void node::feedConnection(unsigned long long int inputID, double input)
{
}

int node::getEnabledConnectionsCount()
{
	int result{ 0 };
	for (auto* c : this->connections) {
		if (c->enabled) {
			result++;
		}
	}
	return result;
}

node::~node()
{
	// clear memory
	for (auto* c : this->connections) {
		delete c;
	}
}
