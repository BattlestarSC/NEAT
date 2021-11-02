#include "pch.h"
#include "individual.h"

individual::individual(int in, int out, hyperparameters* s_hp)
{
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
