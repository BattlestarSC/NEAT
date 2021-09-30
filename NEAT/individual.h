//
// Created by battlestarsc on 9/29/2021.
//

#ifndef NEAT_INDIVIDUAL_H
#define NEAT_INDIVIDUAL_H
#include <list>
#include <vector>
#include "hyperSingleton.h"

/*
 * So, for complete code refactor, we are gonna treat nodes and connections as one
 * A node doesn't exist if no valid and enabled connections to it exist
 * This does preclude two interesting future options, node biases and evolving node activation functions
 *
 * This structure contains all important information
 * One very important distinction to make, ALL INPUT NODES AND OUTPUT NODES HAVE FIXED NUMBERS
 * INPUTS WILL ALWAYS BE 0 ... INPUTSIZE - 1
 * OUTPUT WILL ALWAYS BE INPUTSIZE ... INPUTSIZE + OUTPUTSIZE
 */
struct connection {
    unsigned long long int inputNode;
    unsigned long long int outputNode;
    double weight;
    bool enable;
    unsigned long long int innovationNumber;
};

class individual {
public:
    //create a new individual randomly
    individual(int inSize, int outSize, hyperSingleton* controlSingleton);
    // for asexual reproduction (clone or mutate only)
    explicit individual(individual* cloneParent, bool mutate = false);
    // for sexual reproduction (makes itself based on its provided parents, does not film ;))
    individual(individual* parentA, individual* parentB);
    // feed forward the net
    std::list<double> feed(std::list<double> input);
    // we can take either
    std::vector<double> feed(std::vector<double> input);
    // depending upon type of learning, this will work for both reinforcement and standard learning
    void addFitness(double fit);
    // accessor method to keep shit private
    double getFitness();

    ~individual();

private:
    double fitness = 0.0;
    int inputSize;
    int outputSize;
    std::list<struct connection*> connections;
    hyperSingleton* parameterSingleton = nullptr;


    void crossover(individual* parentA, individual* parentB);
    void mutate();
    // make life easy during feeding, this is the activation function
    double activate();

    // activation functions
    double sigmoid();
    double steepSigmoid();
    double relu();
    double linear();
    double oneThreshold();
};


#endif //NEAT_INDIVIDUAL_H
