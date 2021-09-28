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
        g_current_node = inSize + outSize;
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

// Compare conditions for sort of vectors
bool compInno(conGene* a, conGene* b) {
    if (!a || !b) {
        return false;
    }
    if (b->innovationNumber > a->innovationNumber) {
        return true;
    }
    return false;
}

// First, make sure that the assumption of being more fit is valid
// but rather than do anything that would be challenging, like tossing errors, just print debug and return null
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
// I'm gonna start off with method A to reduce excess code and I can just use the clone function
genome *genome::crossover(genome *weakParent) {
    if(this->fitness < weakParent->fitness) {
        std::cout << "WARNING: invalid parent matching, someone was dating way out of their league" << std::endl;
        return nullptr;
    }

    // copy myself
    auto* baby = this->clone();

    /*
     * So this is the smart way of doing it, but its honestly easier just to waste CPU resources and brute force it, so TOO BAD
    // alright, so, lets crossover matching shit.
    // TODO: the method for this is to sort our vectors of connections by innovation number (0...n) then iterate though
    std::sort(baby->connections.begin(),  baby->connections.end(), compInno);
    // This likely wastes CPU time, but again, whatever
    std::sort(weakParent->connections.begin(),  weakParent->connections.end(), compInno);
    */

     // brute force iterate method
    // for every baby gene
    for (auto b : baby->connections) {
        // for every weak parent gene
        for (auto p : weakParent->connections) {
            // if they match
            if (p->innovationNumber == b->innovationNumber) {
                // 50% chance of taking the weakass parent's genes
                // Since g_unif(g_re) returns a double range [-1.0, 1.0], checking around zero is 50%
                if (g_unif(g_re) > 0.0) {
                    b->weight = p->weight;
                    b->enable = p->enable;
                }

                // this could be a hyper parameter, but, meh, for right now, it can be hardcoded to the paper's values
                // TODO: make this a hyperparameter, and stop being lazy
                if (!b->enable && g_unif(g_re) > 0.5) { // if disabled, 25% chance of reenable
                    b->enable = true;
                }

                // Only one gene should match. If not, we got some really fucked up kids here
                break;
            }
        }
    }

    return baby;
}

// Mutate helper function for a new node
// Split a current connection, add the node, and connect a weight of 1.0 to input and keep the old weight for output
// then disable the connection
void genome::mutate_newNode() {
    // First pick a connection to break
    conGene* broken = nullptr;
    // so pick a random number within the innovation range
    // if its enabled, thats the connection we break. Keep going till we get a valid one
    // yes, worst case O() is O(inf) since its a loop based on randomness, but fuck it, the odds of
    //      an inf loop on a random variable is really small
    // yes, this is shitty. But its quick to write
    do {
        // this should pick a random innovation number, except the last one, we don't want to break that quite yet
        unsigned long long int atmp = floor(abs(g_unif(g_re)) * (g_current_innovation_number - 1));
        for (auto c : this->connections) {
           if( c->innovationNumber == atmp ) {
               if (c->enable) {
                   break;
               }
               broken = c;
           }
        }
    } while (broken == nullptr);

    // create the two new connections, create the new node, and disable the original
    auto* newNode = new nodeGene(g_current_node, nodeType::Hidden);
    g_current_node++;
    auto* newInput = new conGene(broken->inputNode, newNode->nodeID, 1.0, true, g_current_innovation_number);
    g_current_innovation_number++;
    auto* newOutput = new conGene(newNode->nodeID, broken->outputNode, broken->weight, true, g_current_innovation_number);
    g_current_innovation_number++;
    broken->enable = false;

    // attach new shit
    this->connections.push_back(newInput);
    this->connections.push_back(newOutput);
    this->nodes.push_back(newNode);

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
