//
// Created by battlestarsc on 9/25/2021.
//

#ifndef NEAT_CONGENE_H
#define NEAT_CONGENE_H

/*
 * The goal here is to represent a single connection gene of a genome.
 * This was seperated from a node gene for clarity
 *
 * node numbers can never be negative, so unsigned int were used
 * weight was made a double in order to allow finer adjustment
 * innovation number was made a long long int to allow for this running for
 *      a really long time without overflow
 */

class conGene {

public:
    // What node does this connect from
    unsigned int inputNode;
    // What node does this connect to
    unsigned int outputNode;
    // What is the connection weight
    double weight;
    // enable bit
    bool enable;
    // historical marker
    unsigned long long int innovationNumber;
    // list initialization
    conGene(unsigned  int in, unsigned int out, double w, bool e, unsigned long long int inno) : inputNode(in), outputNode(out), weight(w), enable(e), innovationNumber(inno) {};
    //make life easy copy init
    conGene(conGene* cpy): inputNode(cpy->inputNode), outputNode(cpy->outputNode), weight(cpy->weight), enable(cpy->enable), innovationNumber(cpy->innovationNumber){};
};


#endif //NEAT_CONGENE_H
