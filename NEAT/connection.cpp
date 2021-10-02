#include "pch.h"
#include "connection.h"
#include <cctype>
#include <random>

connection::connection(unsigned long long int in, unsigned long long int out, unsigned long long int innovation)
{
	// for details see connection.h comments
	this->inputNode = in;
	this->outputNode = out;
	this->innovationNumber = innovation;
	this->enabled = true;
	// C++ randomness
	// see: https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
	std::random_device rd;  // Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
	std::uniform_real_distribution<> dis(-1.25, 1.25);
	this->weight = dis(gen);
}

connection::connection(connection* cpy)
{
	this->innovationNumber = cpy->innovationNumber;
	this->inputNode = cpy->inputNode;
	this->outputNode = cpy->outputNode;
	this->enabled = cpy->enabled;
	this->weight = cpy->weight;
}

double connection::input(double value)
{
	return value * this->weight;
}
