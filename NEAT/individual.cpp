#include "pch.h"
#include "individual.h"
#include "hyperSingleton.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <cmath>
#include <tuple>

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
    // handle innovation numbers
    auto it = this->connections.begin();
    for (unsigned long long int i = 0; i < this->connections.size(); i++) {
        (*it)->innovationNumber = i;
        std::advance(it, 1);
    }

    // handle globals
    if (this->parameterSingleton->innovationNumber == 0) {
        this->parameterSingleton->innovationNumber = inSize * outSize;
    }
    if (this->parameterSingleton->nodeNumbers == 0) {
        this->parameterSingleton->nodeNumbers = inSize + outSize;
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
double individual::sigmoid(double i) {
    double d = std::exp(i);
    d += 1.0;
    d = 1.0 / d;
    return d;
}

// steepened sigmoid referenced in NEAT paper
double individual::steepSigmoid(double i) {
    double d = -4.9 * i;
    d = std::exp(d);
    d += 1.0;
    d = 1.0 / d;
    return d;
}

// relu activation function
double individual::relu(double i) {
    return (i > 0.0) ? i : 0.0;
}

double individual::linear(double i) {
    return i;
}

double individual::oneThreshold(double i) {
    return (i > 1.0) ? 1.0 : 0.0;
}

double individual::activate(double in) {
    // fallback case
    if (this->parameterSingleton == nullptr) {
        return steepSigmoid(in);
    }
    // actually run activation
    // breaks are not used since EVERY case returns
    switch (this->parameterSingleton->function) {
    case activationFunction::sigmoid:
        return this->sigmoid(in);
    case activationFunction::steepSigmoid:
        return this->steepSigmoid(in);
    case activationFunction::relu:
        return this->relu(in);
    case activationFunction::linear:
        return this->linear(in);
    case activationFunction::oneThreshold:
        return this->oneThreshold(in);
    default:
        // THIS STATE IS UNREACHABLE. IF IT IS REACHED, THE CODE IS IMPOSSIBLY FUCKED. GIVE UP, CRY, AND SEEK PROFESSIONAL HELP. DO NOT ATTEMPT TO FIX
        std::cout << "Reached impossible state in individual::activate(), defaulting to steep sigmoid activation function" << std::endl;
        return steepSigmoid(in);
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

/*
* So we will linearly scan each connection gene
* With the exception of the input nodes, the input of each gene is the sum of weights times activation function outputs
* We will simply trust the above setup to not let things get out of order
* We will keep a running vector of tuples in the following layout
* tuple<unsigned long long int, double> for tuple<node, value>
* When used as an input, if the node value is not in the input range, we use weight*activate(std::get<1>(correct tuple))
*/
std::vector<double> individual::feed(std::vector<double> input) {
    std::vector<std::tuple<unsigned long long int, double>> runtime{};

    // First add all inputs
    for (unsigned long long int i = 0; i < this->inputSize; i++) {

        // check for failure
        if (input.size() >= i) {
            std::cout << "WARNING: Feeding inputs failed, inputs too short" << std::endl;
        }

        runtime.push_back(std::tuple<unsigned long long int, double>(i, input[i])); // damn is this verbose

    }

    // now run though all connections
    for (auto* c : this->connections) {

        // first get input connection
        double input{0.0};
        for (auto i : runtime) {
            if (std::get<0>(i) == c->inputNode) {
                // check if sensor node
                if (std::get<0>(i) < this->inputSize) {
                    input = std::get<1>(i);
                }
                else {
                    input = activate(std::get<1>(i));
                }
                // should only be one entry
                break;
            }
        }

        // now see if there is a current entry for the output node
        std::tuple<unsigned long long int, double>* outNode = nullptr;
        for (auto i : runtime) {
            if (std::get<0>(i) == c->outputNode) {
                outNode = &i;
                break;
            }
        }

        // apply weight
        input *= c->weight;

        // output
        if (outNode == nullptr) {
            runtime.push_back(std::tuple<unsigned long long int, double>{c->outputNode, input});
        }
        else {
            std::get<1>(*outNode) += input; // update the value
        }

    }

    // prepare output vector
    // question, if not created with new and returned as a pointer, will this go out of scope?
    std::vector<double> output{};
    std::vector<std::tuple<unsigned long long int, double>*> outputPrep{};

    for (auto i : runtime) {
        //check if in output range
        if (std::get<0>(i) >= this->inputSize && std::get<0>(i) < this->inputSize + this->outputSize) {
            // add to output
            outputPrep.push_back(&i);
        }
    }

    // correctly order output
    std::sort(outputPrep.begin(), outputPrep.end());

    // load into output 
    for (auto i : outputPrep) {
        output.push_back(std::get<1>(*i));
    }

    return output;
}

// Just return memory nicely
individual::~individual() {
    for (auto* i : this->connections) {
        delete i;
    }
}

