//
// Created by battlestarsc on 9/25/2021.
//

#ifndef NEAT_NODEGENE_H
#define NEAT_NODEGENE_H
#include "geneEnums.h"

/*
 * The goal of this class is to implement a node gene as described by the NEAT spec
 *
 * This node is used for runtime evaluation.
 * The node will receive all weighted inputs and store them
 * When queried for output, it will return the stored output, or calculate the values
 * Since connections will be implemented linearly, there should never be an early request for output
 *
 * FUTURE WORK/NOT Implemented
 * softmax not implemented yet
 * the activationFunction and bias are future thoughts of mine, not mentioned in the NEAT paper
 *      as such, they are not yet implemented in evolution, though to support future implementation
 *      they are employed as dummy functions (i.e. bias is always and the activation function is a static
 *      hyperparameter and doesn't change)
 */

class nodeGene {

public:
    // the node's unique ID
    unsigned int nodeID;
    // if the node is input, output, or hidden
    nodeType type;
    // initalizer
    nodeGene(unsigned int ID, nodeType t, double b = 0.0) :nodeID(ID), type(t), bias(b){};

    // TODO: NOT IMPLEMENTED
    activationFunction actFunct = activationFunction::sigmoid;
    double bias = 0.0f;

// RUNTIME
private:
    bool outputCalcuated = false;
    double output = 0.0f;
    double summedInputs = 0.0f;
    double sigmoid();
    double steepSigmoid();
    double relu();
    double softmax();
    double step();

public:
    double getOutput();
    void input(double value);
};


#endif //NEAT_NODEGENE_H
