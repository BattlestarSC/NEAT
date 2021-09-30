//
// Created by battlestarsc on 9/29/2021.
//

#include "individual.h"
#include "hyperSingleton.h"
#include <iostream>
#include <cmath>

/*
 * So for sorting connections, we want to sort by input node for feeding forward
 */
bool connectionSortFeed(struct connection* a, struct connection* b) {
    // true if a is smaller
    return (a->inputNode <= b->inputNode);
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

