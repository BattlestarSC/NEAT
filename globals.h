//
// Created by battlestarsc on 9/26/2021.
//

#ifndef NEAT_GLOBALS_H
#define NEAT_GLOBALS_H

// Fucking hell, I can't believe I actually found a use for a global I
// am not entirely sure if there is a better option to avoid
extern unsigned long long int g_current_innovation_number = 0;
extern unsigned int g_current_node = 0;
// This seems better than doing it a trillion times
// Yes, putting an include here sucks. deal with it
#include <random>
std::uniform_real_distribution<double> g_unif(-1.0, 1.0);
std::default_random_engine g_re;

#endif //NEAT_GLOBALS_H
