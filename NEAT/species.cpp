#include "pch.h"
#include "species.h"
#include <algorithm>
#include "individual.h"
#include <vector>
#include <cmath>

double species::distance(individual* ind)
{
	if (ind == nullptr || this->representative == nullptr) {
		// Error
		return -1;
	}

	// calculate each factor
	double weightRep{ 0 }, connectionRep{ 0 }, nodesRep{ 0 },
		weightInd{ 0 }, connectionInd{ 0 }, nodesInd{ 0 },
		weightDelta{ 0 }, connectionDelta{ 0 }, nodesDelta{ 0 };

	double output = weightDelta * this->rp->hp.weightsCoefficient;
	output += connectionDelta * this->rp->hp.connectionsCoefficient;
	output += nodesDelta * this->rp->hp.nodesCoefficient;

	return output;
}

bool species::addIndividual(individual* applicant)
{
	if (applicant != nullptr && this->distance(applicant) <= this->distanceThreshold) {
		this->members.push_back(applicant);
		return true;
	}
	return false;
}

std::vector<individual*> species::spawn(int amount)
{
	std::vector<individual*> newChildren{};
	for (int i = 0; i < amount; i++) {
		auto* parentA = this->members[rand() % this->members.size()];
		auto* parentB = this->members[rand() % this->members.size()];
		auto* child = new individual(parentA, parentB);
		child->mutate();
		newChildren.push_back(child);
	}
	return newChildren;
}

/// <summary>
/// Sort helper function by parent fitness
/// /// </summary>
/// <param name="a">genome a</param>
/// <param name="b">genome b</param>
/// <returns>true if a is less than or equal to b</returns>
bool fitnessSort(individual* a, individual* b) {
	return (a->fitness <= b->fitness);
}

void species::slaughter(int amount)
{
	std::sort(this->members.begin(), this->members.end(), fitnessSort);
	this->members.erase(this->members.end() - amount, this->members.end());
}

species::~species()
{
	// memory cleanup
	delete this->representative;
}
