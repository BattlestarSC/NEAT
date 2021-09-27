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

//So, because its unlikely this function will be used outside the clone and/or crossover functions, we can assume that
//as such, its almost certain that all the connections and nodes will later be freed, so we need to copy all of them
//yes, this could cause a memory leak.... TOO BAD
//yes, this wastes CPU cycles... its easier to write it this way, and this is not production code, just for learning, so TOO BAD
//is this bad practice? probably. Again, its just for learning
genome::genome(int inSize, int outSize, std::vector<conGene *> initConnections, std::vector<nodeGene *> initNodes) {
    this->inputSize = inSize;
    this->outputSize = outSize;

    // so just flatly copy everything
    for (auto n : initNodes) {
        this->nodes.push_back(new nodeGene(n));
    }
    for (auto c : initConnections) {
        this->connections.push_back(new conGene(c));
    }
}

// why is this a function? for future code clarity, not because its needed
genome *genome::clone() {
    auto* g = new genome(this->inputSize, this->outputSize, this->connections, this->nodes);
    return g;
}

// So, a couple thoughts on how to do this
// Method A)
//      copy myself
//      iterate though all matching connections and randomly replace connection genes (edit weights and enable bit)
//      since all disjoint and excess genes are taken from the fitter parent, this should be successful in creating functional offspring
// Method B)
//      create a new empty genome
//      iterate through all matching genes and randomly choose a parent to copy from
//      copy all disjoint and excess genes from self
//      this should also be functional
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
    this->fitness+= amt;
}

double genome::getFitness() {
    return this->fitness;
}
