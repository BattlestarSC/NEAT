//
// Created by battlestarsc on 9/29/2021.
//

#include "individual.h"
#include "hyperSingleton.h"
#include <iostream>
#include <random>
#include <cmath>

// Population start constructor
individual::individual(int inSize, int outSize, hyperSingleton *controlSingleton) {
    this->inputSize = inSize;
    this->outputSize = outSize;
    this->parameterSingleton = controlSingleton;

    for(int i = 0; i < this->outputSize; i++) {
        for(int j = 0; j < this->inputSize; j++) {
            auto* con = new struct connection;
            con->inputNode = j;
            con->outputNode = i;
            con->weight = this->parameterSingleton->getRandom();
            this->connections.push_back(con);
        }
    }
}

individual::individual(individual *cloneParent, bool mutate) {

}

individual::individual(individual *parentA, individual *parentB) {

}

std::list<double> individual::feed(std::list<double> input) {
    return std::list<double>();
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

std::vector<double> individual::feed(std::vector<double> input) {
    return std::vector<double>();
}

// Just return memory nicely
individual::~individual() {
    for (auto* i : this->connections) {
        delete i;
    }
}

