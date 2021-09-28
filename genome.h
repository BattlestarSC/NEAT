//
// Created by battlestarsc on 9/25/2021.
//

#ifndef NEAT_GENOME_H
#define NEAT_GENOME_H

#include "conGene.h"
#include "nodeGene.h"
#include <vector>

/*
 * This is how an individual is specified
 * Vector of node genes
 * Vector of connection genes
 * include a record of individual fitness
 *
 * Also hold a count of input and output nodes
 *
 * For runtime, we will also provide the following functions:
 *      Execution:
 *          - feed: give inputs and return outputs
 *              - For ease, we only ever feed a vector of doubles and return a vector of doubles
 *      Reproduction:
 *          - mutate
 *          - clone
 *          - crossover (as the more fit parent)
 */
class genome {

public:
    // Vector of connection genes
    std::vector<conGene*> connections;
    // Vector of node genes
    std::vector<nodeGene*> nodes;
    // Fitness of the individual
    double fitness = 0.0;
    // how many sensors
    int inputSize;
    // how many outputs
    int outputSize;

public:
    genome(int inSize, int outSize);
    genome(int inSize, int outSize, std::vector<conGene*> initConnections, std::vector<nodeGene*> initNodes);
    ~genome();
    genome* clone();
    void mutate(float weightsProbability, float connectionProbability, float nodeProbability, float activationProbability = 0.0);
    genome* crossover(genome* weakParent);
    void addToFitness(double amt);
    double getFitness();

private:
    // just some helper shit
    void mutate_newNode();
};


#endif //NEAT_GENOME_H
