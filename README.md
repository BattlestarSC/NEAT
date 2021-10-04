<h1>C++ NEAT implmentation for learning purposes only</h1>


<h3>Design</h3>

**Connections** 

see: connection.h

These are represented by a dummy class with included data

```

unsigned long long int inputNode;

	unsigned long long int outputNode;

	double weight;

	unsigned long long int innovationNumber;

	bool enabled;

```
**Nodes**

see: node.h

After refactor, we have a new way of handling these. Each node is an object in an individual. Nodes (except input) are represented by a node in the `connections` `std::vector<node*>`

The reason for this: Each node contains a `std::vector<connection*>` that contains every connection **where connection->outputNode == this node**. Each node also contains a running total of its weighted inputs. 

To run the graph, each node is in order in the node vector of the individual. Therefore, we can just execute each node in the vector order, since they each contain the entire list of input connections, we can just scan for all required inputs and feed those individually. 

To get each node's output, we have to get the node->getOutput() function (which runs the activation funtion as well). 

To sort the node order, we just need to ensure each node is before all other nodes that depend on it. Therefore we can also use this order to easily check for valid new connections to mutate, since a new connection would only be valid if it is from an earlier node in the vector and doesn't exist.

**Individuals**

see individual.h

Each individual is make of a vector of nodes, in execution order. In other words, a three layer fully connected graph with 3 nodes per layer would have a vector of 6 nodes (input nodes are not included). Assuming each node has an ID assigned linearly (layer 1 has 0,1,2, layer 2 has 3,4,5, layer 3 has 6,7,8), then the vector would contain nodes in the order 0,1,2,3,4,5,6,7,8

individual.h also contains the structures for containing all hyperparamters and for sharing universal/global numbers/IDs

For example, since innovation numbers are globally shared, and we don't want repetitions (i.e. all new connections from 1 to 3 share the same innovation number), we keep a list of all new connections in the structure so that these can be corralated. 

**Species**

see species.h

So after each generation executes, we divide the population into species based on their distnace function and the threshold. We then execute the bottom portion (decided by hyper parameters) and then we breed new individuals (up to population allocated size (decided by species shared fitness and hyperparamters)). All execution and breeding is done WITHIN EACH SPECIES. This is a core part of the NEAT algorithm

**Population**

see population.h

This is the class that controls the entire population, sets up everything and controls all hyperparamters. I.E. we use this class to run the algorithm

It will also contain the fitness function and be able to give out pointers to any and all individuals