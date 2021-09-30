#include "pch.h"
#include "hyperSingleton.h"
#include <chrono>
#include <cmath>

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

double hyperSingleton::getRandomRange(double a) {
    if (this->seed_val == 0) {
        // seed with ms since epoch
        this->seed_val = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        this->rng.seed(this->seed_val);
    }

    double ran = this->normal_dist(this->rng);
    if (abs(ran) > a) {
        ran = std::fmod(ran, a);
    }
    return ran;
}

double hyperSingleton::getRandom() {
    if (this->seed_val == 0) {
        // seed with ms since epoch
        this->seed_val = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        this->rng.seed(this->seed_val);
    }

    return this->normal_dist(this->rng);
}
