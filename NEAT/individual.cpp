#include "pch.h"
#include "individual.h"
#include "hyperSingleton.h"
#include <iostream>
#include <random>
#include <algorithm>
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
            con->outputNode = this->inputSize+i;
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

// So first, we copy the more fit parent. 
// This causes all excess and disjoint genes to be included automatically
individual::individual(individual* parentA, individual* parentB, bool mutate) {
    // label parents
    auto* fit = (parentA->getFitness() >= parentB->getFitness()) ? parentA : parentB;
    auto* weak = (fit == parentA) ? parentB : parentA;

    // clear our genes, just in case
    this->connections.clear();

    // Copy more fit parent
    this->inputSize = fit->inputSize;
    this->outputSize = fit->outputSize;
    this->parameterSingleton = fit->parameterSingleton;

    for (auto* i : fit->connections) {
        auto* con = new struct connection;
        con->inputNode = i->inputNode;
        con->outputNode = i->outputNode;
        con->weight = i->weight;
        con->enable = i->enable;
        this->connections.push_back(con);
    }

    // Now crossover
    for (auto* c : this->connections) {
        for (auto* w : weak->connections) {
            if (c->innovationNumber == w->innovationNumber) {
                if (this->parameterSingleton->getRandom() < 0.0) {
                    c->enable = w->enable;
                    c->weight = w->weight;

                    // From NEAT paper, during crossover there is a chance of a dead connection being reenabled
                    // They use 25% chance, we will use the hyperparameter enableConnection
                    if (!c->enable) {
                        // So logic exp. The random range generates a random variable in a normal distribution and mods it again its parameter
                        // this means that result could be (-0.5...0.5)
                        // adding 0.5 puts that range (0...1)
                        // so if we have a 25% chance (0.25), then just making it less than that value is the same thing
                        if ((this->parameterSingleton->getRandomRange(0.5) + 0.5) < this->parameterSingleton->enableConnection) {
                            c->enable = true;
                        }
                    }
                }
            }
        }
    }

    if (mutate) {
        this->mutate();
    }

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
* DEPRECATED: This code remains, but has been placed in a slightly updated form in the individual(parent, parent, mutate) breeding constructor
*       update adds connection re-enabling during parents doing the nasty
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

    auto* fitParent = (parentA->getFitness() >= parentB->getFitness()) ? parentA : parentB;
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

/*
* So we define 3 methods of mutation
* 1) connections
* 2) weights
* 3) new node
 */
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

/*
* This function is to be use ONLY when mutating a new connection (new innovation number, etc)
* it has default inputs of 0,0,0.0 for a reason. We can use inputs of (in, out, orig weight) for mutating a new node
* 
* TODO: TEST THIS FUCKING MONSTROSITY
* 
* the critical thing: We need this to insert the new gene after the last output note for its innode and before the first input reference to its outnode
* THIS SHOULD PREVENT CYCLES BEING CREATED
* 
* Since we need a way to make sure innovation numbers for similar structures are not duplicated, we return the new connection for shit to be figured out elsewhere
*/
connection* individual::mutateConnection(unsigned long long int inNode, unsigned long long int outNode, double weight)
{
    unsigned long long int iNode = inNode, oNode = outNode;
    // make the new connection structure
    auto* con = new struct connection;
    // First, if we have (0,0, *) we are creating any brand new connection we want with a random weight
    if (inNode == 0 && outNode == 0) {
        // pick a valid output node
        // we will create a vector of valid indexes
        // we will scan the connections for any non-input node
        std::vector<unsigned long long int> validOutput{};
        // add all valid non-input nodes (this will include output nodes)
        for (auto* c : this->connections) {
            validOutput.push_back(c->outputNode);
        }
        // remove duplicates
        // gotta love stack overflow copy/paste
        // https://stackoverflow.com/questions/1041620/whats-the-most-efficient-way-to-erase-duplicates-and-sort-a-vector
        std::sort(validOutput.begin(), validOutput.end());
        validOutput.erase(std::unique(validOutput.begin(), validOutput.end()), validOutput.end());
        // pick node
        // yes, this is old C style randomness, but whatever, its not overly critical
        oNode = validOutput[rand() % validOutput.size()];

        // now smartly pick an input node
        std::vector<unsigned long long int> validInput{};
        // We will accept any non-duplicate connection that satsifies the following condition
        // it can not exist in the connection genes after the first reference to oNode being used as an input
        for (auto* c : this->connections) {
            // duplicate check
            if (c->outputNode == oNode) {
                continue;
            }
            // too far in genome check
            if (c->inputNode == oNode) {
                break;
            }
            // if this has passed
            validInput.push_back(c->inputNode);
        }

        // if this is empty, we can assume there are few or no valid new connections, so we just give the fuck up
        // yes, this could have the effect of lowering the new connection mutation rate, but its easier to just bump that up
        // than spend more time here
        if (validInput.size() == 0) {
            return nullptr;
        }

        // remove duplicates
       // gotta love stack overflow copy/paste
       // https://stackoverflow.com/questions/1041620/whats-the-most-efficient-way-to-erase-duplicates-and-sort-a-vector
        std::sort(validOutput.begin(), validOutput.end());
        validOutput.erase(std::unique(validOutput.begin(), validOutput.end()), validOutput.end());

        // pick 
        iNode = validInput[rand() % validInput.size()];
    }

    // build the new connection
    con->enable = true;
    con->weight = (weight == 0) ? this->parameterSingleton->getNodeNumber() : weight;
    con->inputNode = iNode; 
    con->outputNode = oNode;
    con->innovationNumber = this->parameterSingleton->getInnovationNumber();

    // insert it
    int lastInputReference = 0, firstOutputReference = 0;
    auto it = this->connections.begin();
    for (int i = 0; i < this->connections.size(); i++) {
        if (firstOutputReference == 0 && (*it)->inputNode == oNode) {
            firstOutputReference = i;
        }
        if ((*it)->outputNode == iNode) {
            lastInputReference = i;
        }
        std::advance(it, 1);
    }

    // Something is REALLY FUCKED UP
    // this SHOULD BE IMPOSSIBLE
    // if this is reached, we just give up and don't add the new connection
    // yes, it may inc the innovation number, but that can take a few extra inc's without breaking
    // this is a BIG problem
    if (firstOutputReference < lastInputReference) {
        return nullptr;
    }

    // decide where to place it (right before first output reference seems sensible)
    // make an iterator to that spot
    it = this->connections.begin();
    std::advance(it, firstOutputReference);
    this->connections.insert(it, con);

    // return the new connection for handling the innovation number shit
    return con;
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

