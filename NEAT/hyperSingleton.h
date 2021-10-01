#pragma once
#include <random>

// random setup from https://stackoverflow.com/questions/7114043/random-number-generation-in-c11-how-to-generate-how-does-it-work
typedef std::mt19937 MyRNG;

/*
 * This is just a singleton to hold hyperparameters
 * It also holds global innovation numbers and node numbers and a random number engine
 */

enum class activationFunction {
    sigmoid,
    steepSigmoid,
    relu,
    linear,
    oneThreshold
};

class hyperSingleton {
public:
    const float mutateWeights;
    const float mutateConnections;
    const float mutateNodes;
    const float enableConnection;
    const int populationSize;
    const double speciesThreshold;
    const float weightsCoefficient;
    const float connectionsCoefficient;
    const float nodesCoefficient;
    const activationFunction function;

    hyperSingleton(float a, float b, float c, float d, int e, double f, float g, float h, float i, activationFunction j = activationFunction::steepSigmoid) : mutateWeights(a), mutateConnections(b), mutateNodes(c), enableConnection(d), populationSize(e), speciesThreshold(f), weightsCoefficient(g), connectionsCoefficient(h), nodesCoefficient(i), function(j) {};

public:
    // Get the new element's number and then increment the counters
    unsigned long long int getInnovationNumber();
    unsigned long long int getNodeNumber();

    double getRandomRange(double a);
    double getRandom();


public:
    unsigned long long int innovationNumber = 0;
    unsigned long long int nodeNumbers = 0;
    uint32_t seed_val = 0;
    MyRNG rng;
    std::normal_distribution<double> normal_dist{ 0, 1 };


};