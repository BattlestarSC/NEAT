#pragma once
#include <vector>
#include <random>
#include "node.h"
#include "connection.h"

/*
* All included mutate options use default parameters, for custom probabilities, use false then call manually
*/
class individual
{
public:
	// global random generator so we don't reseed every time
	std::random_device rd;  // Will be used to obtain a seed for the random number engine
	std::mt19937 gen{ rd() }; // Standard mersenne_twister_engine seeded with rd()

	double fitness{ 0.0 };
	std::vector<node*> nodes;
	int inputSize;
	int outputSize;

	/// <summary>
	/// Copy and/or asexual reproduction constructor
	/// </summary>
	/// <param name="cpy">Lonely Parent</param>
	/// <param name="mutate">Optional: be a good parent and mutate the child? default = false </param>
	individual(individual* cpy, bool mutate = false);
	/// <summary>
	/// Sexual reproduction constructor
	/// Important note, we don not record the act
	/// </summary>
	/// <param name="father">first parent</param>
	/// <param name="mother">second parent</param>
	/// <param name="enable">Probability of a disabled connection being reenabled</param>
	/// <param name="mutate">optional: do we want to mutate the child (like any good parent). Default = false </param>
	individual(individual* father, individual* mother, float enable = 0.25f, bool mutate = false);
	/// <summary>
	/// Create a brand new fully connected but minimial network
	/// </summary>
	/// <param name="inSize">how many inputs</param>
	/// <param name="outSize">how many outputs</param>
	individual(int inSize, int outSize);
	/// <summary>
	/// mutate ourselves according to provided probabilities. 
	/// </summary>
	/// <param name="weightProbability">probability of mutating weights at all</param>
	/// <param name="connectionProbability">probability of mutating a new connection</param>
	/// <param name="nodeProbability">probability of mutating a new node</param>
	/// <param name="weightsUniformProbability">probability of altering weights uniformly vs randomly reassigning all of them </param>
	/// <returns>ALL NEW CONNECTIONS. This is for making sure innovation numbers are universal and not duplicated. </returns>
	std::vector<connection*> mutate(float weightProbability = 0.9f, float connectionProbability = 0.03f, float nodeProbability = 0.005f, float weightsUniformProbability = 0.9f);
	/// <summary>
	/// Don't copy the same code into all copy based constructors
	/// </summary>
	/// <param name="cpy">parent to copy</param>
	void copy(individual* cpy);
	/// <summary>
	/// preturb weights by the same amount
	/// </summary>
	/// <param name="amt">how much to adjust by</param>
	void uniformMutateWeights(double amt);
	/// <summary>
	/// Same deal as uniform, but randomly reassign all
	/// Cleaner to organize this way
	/// </summary>
	void randomMutateWeights();
	/// <summary>
	/// Helper function, true if there are no possible new connections
	/// </summary>
	bool fullyConnected();
	/// <summary>
	/// Gets a new innovation number somehow (yet to be decided on how)
	/// </summary>
	/// <returns>innovation number</returns>
	unsigned long long int getInnovationNumber();
	/// <summary>
	/// Gets a new node number somehow (yet to be decided on how)
	/// </summary>
	/// <returns>node number</returns>
	unsigned long long int getNodeNumber();
	/// <summary>
	/// Feed data through the graph
	/// </summary>
	/// <param name="sense">sensor vector</param>
	/// <returns></returns>
	std::vector<double> feed(std::vector<double> sense);
	/// Free memory on death
	~individual();
};

