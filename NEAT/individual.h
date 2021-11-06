#pragma once
#include <vector>
#include "node.h"
#include "connection.h"
#include <random>

/*
* Used to save all fitness data
* Specific to each implmentation, except for total fitness
*/
struct fitness {
	double fitness;
};

/*
* Used to share all hyperparameters to avoid using a global
*/
typedef struct hyperparamters {
	// hyper parameters
	// these are snake_case to show that they are "constants" and hyperparameters
	double mutate_weights_chance;
	double mutate_weights_uniform;
	double mutate_connection_chance;
	double mutate_node_chance;
	double mutate_connection_reenable;

	/*
	* Random generation, shared so no re-seeding
	* These MUST be initalized as follows
	* std::default_random_engine generator;
	* std::normal_distribution<double> distribution(0.0,1.0);
	* Which will make a random normal distribution with mean of 0 and standard deviation of 1
	* These will be used as follows
	* *distribution(*generator)
	* for a random double
	*/
	std::default_random_engine* generator;
	std::normal_distribution<double>* distribution;

	// shared vectors (warning: I can't remember if vector is thread safe)
	std::vector<connection*> new_connections;
	std::vector<node*> new_nodes;

	// shared number pointers (warning: DEFINATELY not thread safe in current state)
	unsigned long long int* g_next_innovation;
	unsigned long long int* g_next_node;
} hyperparameters;

class individual
{
public:
	// first, the data we need to save
	// technically we should make these private and use getters and setters
	//		but that sounds like extra work and honestly I think makes more sense in a company enviroment than one or two people enviroment, so fuck that shit
	//		also its easier to set up tests this way
	struct fitness;
	std::vector<node*> nodes;
	unsigned int inputSize;
	unsigned int outputSize;
	hyperparameters* hp;
	

	// now all functions
public:
	// initial creation
	individual(int in, int out, hyperparameters* s_hp);
	// sex
	individual(individual* parentA, individual* parentB);
	// clone
	individual(individual* copy);
	// execute
	std::vector<double> feedGraph(std::vector<double> input);
	// fix ordering/sort
	bool reorderNetwork();
	// actually define "the birds and the bees", used during the rough sex used to make a child (sexual constructor)
	// tl;dr individual constructor copies the more fit parent then we cross over with the weaker one, keeping all excess genes
	// we also do connection re-enable here, rather than in mutate
	void crossover(individual* wimpParent);
	// when we want to irradaite a child, use hp probabilities
	void mutate();
	// how we assign fitness, this is unique to each implmentation, but will only use the data from our fitness struct
	// return the fitness and save it in the struct
	double calculateFitness();
	// destroy
	~individual();

	// helper functions
private:
	// DRY principle, used in non-initial constructors
	void copy(individual* parent);
	// mutation helper functions
	void mutateWeights();
	connection* mutateConnection(); //return connection created
	node* mutateNode(); //return node created
	connection* mutateMemory(); // return connection created
};

