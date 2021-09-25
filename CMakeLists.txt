cmake_minimum_required(VERSION 3.20)
project(NEAT)

set(CMAKE_CXX_STANDARD 11)

add_executable(NEAT main.cpp conGene.cpp conGene.h nodeGene.cpp nodeGene.h geneEnums.h genome.cpp genome.h)
