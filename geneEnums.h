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
    softmax
};

#endif //NEAT_GENEENUMS_H
