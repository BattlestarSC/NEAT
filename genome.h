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
 * For runtime, we will also provide the following functions:
 *      Execution:
 *          - feed: give inputs and return outputs
 *      Reproduction:
 *          - mutate
 */
class genome {

public:
    std::vector<conGene*> connections;
    std::vector<nodeGene*> nodes;
    double fitness;

};


#endif //NEAT_GENOME_H
