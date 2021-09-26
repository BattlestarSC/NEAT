//
// Created by battlestarsc on 9/25/2021.
//

#include <iostream>
#include "genome.h"
#include "globals.h"
#include "nodeGene.h"

// So this is ONLY just for initial creation where we do not have parents
// Create a minimal node structure that is fully connected
genome::genome(int inSize, int outSize) {
    // Yes, print debugging, and yes, in production
    if (g_current_innovation_number > inSize*outSize) { // too many innovations for initial creation
        std::cout << "WARNING: IIllegal child manufacturing detected" << std::endl;
    }

    inputSize = inSize;
    outSize = outputSize;

    // first create some nodes
    for(unsigned int i = 0; i < inSize; i++) {
        auto* node = new nodeGene(i, nodeType::Sensor);
        nodes.push_back(node);
    }
    // and outputs
    for(unsigned int i = inSize; i < (inSize + outSize); i++) {
        auto* node = new nodeGene(i, nodeType::Output);
        nodes.push_back(node);
    }

    // now connections
    // this is for all input nodes connected to all nodes, so if 5 and 3, you have 5 * 3 connections
    int numberOfConnections = inSize * outSize;
    // and since this function is ONLY for initial creation
    if (g_current_innovation_number == 0) {
        g_current_innovation_number = numberOfConnections; // if 15, then we have innovations 0...14
    }
    // lets make EVERY SINGLE connection
    for(unsigned int i = 0; i < inSize; i++) {
        for (unsigned int j = 0; j < outSize; j++) {
            double weight = g_unif(g_re);
            auto* connection = new conGene(i, j, weight, true, (i * outSize) + j);
            connections.push_back(connection);
        }
    }
}

genome::genome(int inSize, int outSize, std::vector<conGene *> initConnections, std::vector<nodeGene *> initNodes) {

}

genome *genome::clone() {
    return nullptr;
}

genome *genome::crossover(genome *weakParent) {
    return nullptr;
}

void genome::mutate(float weightsProbability, float connectionProbability, float nodeProbability,
                    float activationProbability) {

}

// Just free all the allocated memory
genome::~genome() {

    for (auto i : nodes) {
        delete i;
    }
    nodes.clear();

    for (auto i : connections) {
        delete i;
    }
    connections.clear();
}

void genome::addToFitness(double amt) {

}

double genome::getFitness() {
    return this->fitness;
}
