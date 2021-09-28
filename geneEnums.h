//
// Created by battlestarsc on 9/25/2021.
//

#ifndef NEAT_GENEENUMS_H
#define NEAT_GENEENUMS_H

enum class nodeType {
    Sensor,
    Output,
    Hidden
};

enum class activationFunction {
    relu,
    sigmoid,
    linear,
    step,
    steepSigmoid,
    // on second thought, this doesn't exactly make sense except for I/O nodes that are in static layers
    // TODO: add this only for I/O layers
    softmax
};

#endif //NEAT_GENEENUMS_H
