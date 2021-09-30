//
// Created by battlestarsc on 9/29/2021.
//

#include "individual.h"
#include "hyperSingleton.h"
#include <iostream>
#include <cmath>

/*
 * So for sorting connections, we want to sort into an order than can be directly fed
 * The further along in the graph node is the one that depends on fewer input nodes
 *
 * std::sort expects this to be true if a is smaller
 *
 * So sort this way:
 * 1) if the connection has an input within the input range, it is smaller
 *      to prevent a non-deterministic sorting order, if both input from input nodes, the smaller node number is smaller
 * 2) if one connection has an output within the output range, it is larger
 *      to prevent a non-deterministic sorting order, if bothy output to output nodes, the smaller node number is smaller
 * 3) if one connection outputs to the other's input, it is smaller
 * 4) um, really not sure here
 */
bool individual::connectionSortFeed(struct connection* a, struct connection* b) {
    // Step 1
    bool aInput = (a->inputNode < this->inputSize);
    bool bInput = (b->inputNode < this->inputSize);
    if (aInput && bInput) {
        return (a->inputNode <= b->inputNode);
    }
    if (aInput) {
        return true;
    }
    if (bInput) {
        return false;
    }

    // Step 2
    bool aOutput = a->outputNode >= this->inputSize && a->outputNode < (this->inputSize + this->outputSize);
    bool bOutput = b->outputNode >= this->inputSize && b->outputNode < (this->inputSize + this->outputSize);
    if (aOutput && bOutput) {
        return (a->outputNode <= b->outputNode);
    }
    if (aOutput) {
        return true;
    }
    if (bOutput) {
        return false;
    }

    //step 3
    if (a->outputNode == b->inputNode) {
        return true;
    }
    if (b->outputNode == a->inputNode) {
        return false;
    }

    //TODO: Figure this sort out
    return true; // placeholder
}

/*
 * So for sorting connections, we want to sort by innovation number for evolution
 */
bool connectionSortInnovation(struct connection* a, struct connection* b) {
    // true if a is smaller
    return (a->innovationNumber < b->innovationNumber);
}

individual::individual(int inputSize, int outputSize, hyperSingleton *controlSingleton) {

}

individual::individual(individual *cloneParent, bool mutate) {

}

individual::individual(individual *parentA, individual *parentB) {

}

std::vector<double> individual::feed(std::vector<double> input) {
    return std::vector<double>();
}

void individual::addFitness(double fit) {
    this->fitness += fit;
}

double individual::getFitness() {
    return this->fitness;
}

void individual::crossover(individual *parentA, individual *parentB) {

}

void individual::mutate() {

}

// standard sigmoid function
double individual::sigmoid() {
    double d = std::exp(- this->fitness);
    d += 1.0;
    d = 1.0 / d;
    return d;
}

// steepened sigmoid referenced in NEAT paper
double individual::steepSigmoid() {
    double d = -4.9 * this->fitness;
    d = std::exp(d);
    d += 1.0;
    d = 1.0/d;
    return d;
}

// relu activation function
double individual::relu() {
    return (this->fitness > 0.0) ? this->fitness : 0.0;
}

double individual::linear() {
    return this->fitness;
}

double individual::oneThreshold() {
    return (this->fitness > 1.0) ? 1.0 : 0.0;
}

double individual::activate() {
    // fallback case
    if (this->parameterSingleton == nullptr) {
        return steepSigmoid();
    }
    // actually run activation
    // breaks are not used since EVERY case returns
    switch (this->parameterSingleton->function) {
        case activationFunction::sigmoid:
            return this->sigmoid();
        case activationFunction::steepSigmoid:
            return this->steepSigmoid();
        case activationFunction::relu:
            return this->relu();
        case activationFunction::linear:
            return this->linear();
        case activationFunction::oneThreshold:
            return this->oneThreshold();
        default:
            // THIS STATE IS UNREACHABLE. IF IT IS REACHED, THE CODE IS IMPOSSIBLY FUCKED. GIVE UP, CRY, AND SEEK PROFESSIONAL HELP. DO NOT ATTEMPT TO FIX
            std::cout << "Reached impossible state in individual::activate(), defaulting to steep sigmoid activation function" <<std::endl;
            return steepSigmoid();
    }
}

