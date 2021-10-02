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

