//
// Created by battlestarsc on 9/25/2021.
//

#include "nodeGene.h"
#include <cmath> // for exp
#include <algorithm> // for std::max

// classic sigmoid activation function
double nodeGene::sigmoid() {
    double d = std::exp(- this->summedInputs);
    d += 1.0;
    d = 1.0 / d;
    return d;
}

// steepened sigmoid referenced in NEAT paper
double nodeGene::steepSigmoid() {
    double d = -4.9 * this->summedInputs;
    d = std::exp(d);
    d += 1.0;
    d = 1.0/d;
    return d;
}

// relu activation function
double nodeGene::relu() {
    return std::max(0.0, this->summedInputs);
}

double nodeGene::softmax() {
    return 0;
}

// classic dummy step function
double nodeGene::step() {
    double out = 0.0;
    if (this->summedInputs >= 1.0) {
        out = 1.0;
    } else {
        out = 0.0;
    }
    return out;
}

// Logic: check if output is ready. If so, return it
// if not, apply the correct activation function and then
// return that and update the calcuated value
// make sure to add the bias
double nodeGene::getOutput() {
    // don't do extra work
    // if called early, then we go on strike
    if (this->outputCalcuated) {
        return this->output;
    }

    // run activation and return
    switch(this->actFunct) {
        case activationFunction::sigmoid:
            this->output = this->sigmoid();
            break;
        case activationFunction::steepSigmoid:
            this->output = this->steepSigmoid();
            break;
        case activationFunction::relu:
            this->output = this->relu();
            break;
        case activationFunction::softmax:
            this->output = this->softmax();
            break;
        case activationFunction::step:
           this->output = this-> step();
            break;
        case activationFunction::linear:
            this->output = this->summedInputs;
            break;
        default:
            // arbitrary catchall.
            //  THIS CODE IS UNREACHABLE IF HIT DURING RUNTIME, GIVE UP, CRY, AND SEEK COUNSELING
            this->output = this->sigmoid();
    }
    // while not implemented, this just wastes time adding 0.0f;
    this->output = this->output + this->bias;

    // flip calculated
    this->outputCalcuated = true;

    // return
    return this->output;
}

// add an input value
void nodeGene::input(double value) {
    this->summedInputs += value;
    return; // here for clarity
}
