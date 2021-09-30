#include "pch.h"
#include "individual.h"
#include "hyperSingleton.h"
#include <iostream>
#include <random>
#include <cmath>

// Population start constructor
individual::individual(int inSize, int outSize, hyperSingleton* controlSingleton) {
    this->inputSize = inSize;
    this->outputSize = outSize;
    this->parameterSingleton = controlSingleton;

    for (int i = 0; i < this->outputSize; i++) {
        for (int j = 0; j < this->inputSize; j++) {
            auto* con = new struct connection;
            con->inputNode = j;
            con->outputNode = i;
            con->weight = this->parameterSingleton->getRandom();
            con->enable = true;
            this->connections.push_back(con);
        }
    }
}

// This can be used to preserve an individual across generations or to create a child
// using only mutation
individual::individual(individual* cloneParent, bool mutate) {
    this->inputSize = cloneParent->inputSize;
    this->outputSize = cloneParent->outputSize;
    this->parameterSingleton = cloneParent->parameterSingleton;

    for (auto* i : cloneParent->connections) {
        auto* con = new struct connection;
        con->inputNode = i->inputNode;
        con->outputNode = i->outputNode;
        con->weight = i->weight;
        con->enable = i->enable;
        this->connections.push_back(con);
    }

    if (mutate) {
        this->mutate();
    }
}

individual::individual(individual* parentA, individual* parentB) {

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

/*
 * So the important assumption is that both parentA->connections and parentB->connections are in the same order. Therefore,
 * omitted innovation numbers aren't shared and each genome only needs a single linear scan in theory best case time
 * We will actually do it in nested wasteful for loops for ease
 */
void individual::crossover(individual* parentA, individual* parentB) {

    /*
     * First, lets copy everything from the more fit parent
     * This will solve making sure all excess and disjoint genes are included
     * and it will maintain proper execution order
     */

    auto* fitParent = (parentA->fitness >= parentB->fitness) ? parentA : parentB;
    auto* weakParent = (fitParent == parentA) ? parentB : parentA;

    // clear our genes
    this->connections.clear();

    // copy
    for (auto* i : fitParent->connections) {
        auto* con = new struct connection;
        con->inputNode = i->inputNode;
        con->outputNode = i->outputNode;
        con->weight = i->weight;
        con->enable = i->enable;
        this->connections.push_back(con);
    }

    // now crossover matching genes from the less fit parent
    for (auto* g : weakParent->connections) {
        for (auto* c : this->connections) {
            if (g->innovationNumber == c->innovationNumber) {
                // 50% chance to take weak parent's genes
                if (this->parameterSingleton->getRandom() < 0.0) {
                    c->weight = g->weight;
                    c->enable = g->enable;
                }
            }
        }
    }


}

void individual::mutate() {

}

// standard sigmoid function
double individual::sigmoid() {
    double d = std::exp(-this->fitness);
    d += 1.0;
    d = 1.0 / d;
    return d;
}

// steepened sigmoid referenced in NEAT paper
double individual::steepSigmoid() {
    double d = -4.9 * this->fitness;
    d = std::exp(d);
    d += 1.0;
    d = 1.0 / d;
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
        std::cout << "Reached impossible state in individual::activate(), defaulting to steep sigmoid activation function" << std::endl;
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

