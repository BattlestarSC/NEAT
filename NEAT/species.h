#pragma once
#include <vector>
#include "individual.h"
/*
* A species in the NEAT algorithm
*/
class species
{
public:
	double weightsCoefficient;
	double connectionsCoefficient;
	double nodesCoefficient;
	double distanceThreshold;
	unsigned int populationSize;
	double totalFitness;
	struct runtimeParameters* rp;

	/// <summary>
	/// create a species from parameters and a representative
	/// </summary>
	/// <param name="ind">representative</param>
	/// <param name="wc">weights coefficient from distance function</param>
	/// <param name="cc">connections coefficient from distance function</param>
	/// <param name="nc">nodes coefficient from distance function</param>
	/// <param name="dt">distance threshold of species from distance function</param>
	/// <param name="ps">max population size (used to determine how many offspring are permitted)</param>
	species(individual* ind, double wc, double cc, double nc, double dt, unsigned int ps, struct runtimeParameters* r) : representative(ind), 
		weightsCoefficient(wc),
		connectionsCoefficient(cc),
		nodesCoefficient(nc),
		distanceThreshold(dt),
		populationSize(ps),
		rp(r){};

	/// <summary>
	/// How far is this individual from this->representative?
	/// </summary>
	/// <param name="ind">individual</param>
	/// <returns>double distance</returns>
	double distance(individual* ind);

	/// <summary>
	/// Do we add this individual to the popoulation?
	/// If so, add them, else return false. 
	/// Based upon the distance threshold
	/// </summary>
	/// <param name="applicant">individual considering being in the population</param>
	/// <returns>permitted or not</returns>
	bool addIndividual(individual* applicant);

	/// <summary>
	/// Create amount offspring and return them
	/// </summary>
	/// <param name="amount">how many offspring do we want</param>
	/// <returns>vector of babies</returns>
	std::vector<individual*> spawn(int amount);

	/// <summary>
	/// How many of the lower fit individual don't get to reproduce
	/// </summary>
	/// <param name="amount"></param>
	void slaughter(int amount);

	individual representative;
	std::vector<individual*> members;
};

