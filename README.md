<h1>C++ NEAT implmentation with modification to add memory</h1>


<h2>Unsolved</h2>

1. How to properly universally share a singleton without dependency error
2. How to correct innovation and node numbers 

<h3>Design</h3>

<h4>**Connections**</h4>

see: connection.h

These are represented by a dummy class with included data

```

	unsigned long long int inputNode;

	unsigned long long int outputNode;

	double weight;

	unsigned long long int innovationNumber;

	bool enabled;

	bool memory;

```
<h4>**Nodes**</h4>

see: node.h

After refactor, we have a new way of handling these. Each node is an object in an individual. Nodes (except input) are represented by a node in the `connections` `std::vector<node*>`

The reason for this: Each node contains a `std::vector<connection*>` that contains every connection **where connection->outputNode == this node**. Each node also contains a running total of its weighted inputs. 

To run the graph, each node is in order in the node vector of the individual. Therefore, we can just execute each node in the vector order, since they each contain the entire list of input connections, we can just scan for all required inputs and feed those individually. 

To get each node's output, we have to get the node->getOutput() function (which runs the activation funtion as well). 

To sort the node order, we just need to ensure each node is before all other nodes that depend on it. Therefore we can also use this order to easily check for valid new connections to mutate, since a new connection would only be valid if it is from an earlier node in the vector and doesn't exist.

Each node will also keep two pieces of data for memory support. 

1. A std::vector<connection*> of recurrent connections. On the first execution of a graph, these always take a 0 as input. Afterwards, they use the 2nd item on this list for their input, from their input node
2. A double of their previous output. This is used for RNN connections

Each node will have the following functions (not including supporting functions)

1. addInput(connection*, value): This will add the value of an input times its weight to the node's running talley. Value will have a default value of 0.0, in the event that a memory connection is unable to find a past value (THIS SHOULD CAUSE AN ERROR/TEXT OUTPUT AND KILL OFF THE INDIVIDUAL)
2. getOutput(past): past is a boolean with a default of false. This returns the result of the activation function with the node's current total. If past is true, we are working with a memory connection, so we use the last outoput instead
3. Destructor(): Since all connections are unique and allocated upon construction, we need to free all that memory

<h4>**Individuals**</h4>

see individual.h

Each individual is make of a vector of nodes, in execution order. In other words, a three layer fully connected graph with 3 nodes per layer would have a vector of 6 nodes (input nodes are not included). Assuming each node has an ID assigned linearly (layer 1 has 0,1,2, layer 2 has 3,4,5, layer 3 has 6,7,8), then the vector would contain nodes in the order 0,1,2,3,4,5,6,7,8

Each individual tracks the following relevant variables. 

1. Fitness
2. Every fitness detail {time alive, ships shot down, etc}

Additionally, each individual will, in an extreme effort to avoid using any globals, keep a complete list of all relevant hyper parameters. 

1. percent chance of mutating weights
2. percent chance of mutating weights uniformly
3. percent chance of mutating a new connection
4. percent chance of mutating a new node
5. percent chance of reenabling a disabled connection
6. pointer to a function that returns a uniform random double such that the random number generator is universally shared
7. pointer to a vector of connections for new connections of the generation (provided by population, for making sure that innovation numbers of similar developments are shared)
8. pointer to a vector of nodes for new nodes of the generation (provided by population, for making sure that node numbers are shared correctly)
9. a unsigned long long int of the most recent innovation number (so that a new connection can be assigned, then this incremented, then corrected after all evolution)
10. a unsigned long long int of the most recent node number (for the same reason)

Functions:

1. FeedGraph(std::vector<double> input): feed an input through the network and return its output
2. Destructor(): Since all nodes and connections are unique and allocated upon construction, we need to free all that memory
3. Reorder network(): Reorder the nodes in the network to ensure a new valid graph and easy reproduction. We order with input nodes first, then we ensure that a node is in the list after all input connections to it are executed but before its first output. To break ties, lower node numbers come first. Connections are reordered in innovation node number order.
4. sexual constructor(parentA*, parentB*): this creates a child via crossover and mutation from 2 parents. All disjoint and excess connections are kept. 
5. crossover(parentA*, parentB*): this performs the crossover function of reproduction. We copy the more fit parent, then go through the list of all less fit parent's nodes and connections. Non-matches are kept, otherwise 50% chance of taking the weight (change the weight and enable bit of the copyied gene). There is a percent chance that a disabled connection will be reenabled.
6. default constructor(input size, output size): creates a default fully connected network with no memory. Nodes 0...inputSize-1 are input and inputSize...outputSize-1 are output. All innovation numbers are standardized and weights are randomly assigned from a normal distribution with mean of 0 and variance of 1
7. mutate(): mutates the child. We have a chance (hyperparameter) of mutating weights, connections, nodes, and memory connections
8. mutateWeights(): If we mutate weights, we have a chance (also different hyperparameter) to uniformly preturb all weights by a random amount (-0.25,0.25) or randomly reassign all weights
9. mutateConnection(): Find a list of all possible new connections, randomly pick one, and then create it with a new innovation number. Forward only. Random new weight, like when creating a default network
10. mutateMemoryConnection(): Same as mutate connection, but for memory connections. Connections are allowed both forward and back, but not to input nodes.
11. mutateNewNode(): Select a random connection, disable it, add a new node with a new number, add an input connection with a weight of 1.0 and an output connection with a weight of the disabled connection
12. fitness function(): calcuates fitness from all details of fitness

<h4>**Species**</h4>

see species.h

So after each generation executes, we divide the population into species based on their distnace function and the threshold. We then execute the bottom portion (decided by hyper parameters) and then we breed new individuals (up to population allocated size (decided by species shared fitness and hyperparamters)). All execution and breeding is done WITHIN EACH SPECIES. This is a core part of the NEAT algorithm

Data:

1. a pointer to its representative (copied and newly allocated)
2. size
3. total fitness
4. std::vector of pointers to members

Functions:

1. Constructor(representative): copy the representative and add its fitness to the total fitness
2. Destructor(): free representative AND ALL MEMBERS SINCE THE NEW ONES SHOULD BE ADDED TO THE TOTAL POPULATION BY THE RETURN VECTOR
3. execute the weak (percent): self explainatory 
4. repopulate(number): how many children to create. Return a vector of pointers
5. get shared fitness (): return totalFitness/size (avg)
6. add member(): add the member to the list and add its fitness

<h4>**Population**</h4>

see population.h

This is the class that controls the entire population, sets up everything and controls all hyperparamters. I.E. we use this class to run the algorithm

Since the list of members are public and iteraterable, they are fed manually by the connected framework, so no feed function is needed

Data and hyperparamters:

1. percent chance of mutating weights
2. percent chance of mutating weights uniformly
3. percent chance of mutating a new connection
4. percent chance of mutating a new node
5. percent chance of reenabling a disabled connection
6. a function that returns a uniform random double such that the random number generator is universally shared
7. a vector of connections for new connections of the generation (provided by population, for making sure that innovation numbers of similar developments are shared)
8. a vector of nodes for new nodes of the generation (provided by population, for making sure that node numbers are shared correctly)
9. a unsigned long long int of the most recent innovation number (so that a new connection can be assigned, then this incremented, then corrected after all evolution)
10. a unsigned long long int of the most recent node number (for the same reason)
11. distance threshold
12. distance weights coefficient
13. distance connections coefficient
14. distance memory connections coefficient
15. distance node connections coefficient
16. a vector of all members of the population
17. a vector of all species 
18. max population size

Functions:

1. Constructor(): set the hyperparameters and spawn the entire population
2. Destructor(): free all allocated memory and individuals
3. speciate()
4. breed species()