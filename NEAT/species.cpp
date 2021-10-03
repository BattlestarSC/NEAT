#include "pch.h"
#include "species.h"

double species::distance(individual* ind)
{
	return 0.0;
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
	return std::vector<individual*>();
}
