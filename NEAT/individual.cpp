#include "pch.h"
#include "individual.h"

individual::individual(int in, int out, hyperparameters* s_hp)
{
	inputSize = in;
	outputSize = out;
	hp = s_hp;

	// Fucking C++ random number generation
	std::normal_distribution<double> d = *this->hp->distribution;
	std::default_random_engine g = *this->hp->generator;

	for (unsigned long long int outputNode = 0; outputNode < out; outputNode++) {
		std::vector<connection*> newNodeConnections{};
		for (unsigned long long int inputNode = 0; inputNode < in; inputNode++) {

			/*
			* This monster generates a new connection. 
			* So, because 0...inputSize -1 are input nodes and inputSize... inputSize+outputSize -1 are output nodes, we have to do some fancy math
			* we also need to generate a weight and calcuate a unique innovation number
			* Then make the node and add it to the vector
			*/
			unsigned long long int outNumber = inputSize + outputNode;
			double weight{ d(g) };
			unsigned long long int innoNumber = (outputNode * inputSize) + inputNode;
			connection* c = new connection{inputNode,
									outNumber,
									weight,
									innoNumber,
									true,
									false};
			newNodeConnections.push_back(c);
		}
		// now make the new node
		node* n = new node{ newNodeConnections, inputSize + outputNode };
		// add it
		this->nodes.push_back(n);
		// and free the created connections
		for (auto* c : newNodeConnections) {
			delete c;
		}
	}
}

individual::individual(individual* parentA, individual* parentB)
{
}

individual::individual(individual* copy)
{
}

std::vector<double> individual::feedGraph(std::vector<double> input)
{
	return std::vector<double>();
}

bool individual::reorderNetwork()
{
	return false;
}

void individual::crossover(individual* wimpParent)
{
}

void individual::mutate()
{
}

double individual::calculateFitness()
{
	return 0.0;
}

individual::~individual()
{
}

void individual::copy(individual* parent)
{
}

void individual::mutateWeights()
{
}

connection* individual::mutateConnection()
{
	return nullptr;
}

node* individual::mutateNode()
{
	return nullptr;
}

connection* individual::mutateMemory()
{
	return nullptr;
}
