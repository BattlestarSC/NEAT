//
// Created by battlestarsc on 9/29/2021.
//

#include "hyperSingleton.h"

unsigned long long int hyperSingleton::getInnovationNumber() {
    unsigned long long int output = innovationNumber;
    innovationNumber++;
    return output;
}

unsigned long long int hyperSingleton::getNodeNumber() {
    unsigned long long int output = nodeNumbers;
    nodeNumbers++;
    return output;
}
