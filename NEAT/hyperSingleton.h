//
// Created by battlestarsc on 9/29/2021.
//

#ifndef NEAT_HYPERSINGLETON_H
#define NEAT_HYPERSINGLETON_H
/*
 * This is just a singleton to hold hyperparameters
 * It also holds global innovation numbers and node numbers
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

    hyperSingleton(float a, float b, float c, float d, int e, double f, float g, float h, float i, activationFunction j = activationFunction::steepSigmoid): mutateWeights(a), mutateConnections(b), mutateNodes(c), enableConnection(d), populationSize(e), speciesThreshold(f), weightsCoefficient(g), connectionsCoefficient(h), nodesCoefficient(i), function(j){};

public:
    // Get the new element's number and then increment the counters
    unsigned long long int getInnovationNumber();
    unsigned long long int getNodeNumber();

private:
    unsigned long long int innovationNumber = 0;
    unsigned long long int nodeNumbers = 0;
};


#endif //NEAT_HYPERSINGLETON_H
