<h1>C++ NEAT implmentation for learning purposes only</h1>


<h3>Design</h3>

**Connections** 

see: individual.h

These are represented by a structure that contains all the relevant data 

```
struct connection {

unsigned long long int inputNode;

unsigned long long int outputNode;

double weight;

bool enable;

unsigned long long int innovationNumber;

};
```
**Nodes**

see: individual.h

These are **not implemented** instead they are based entirely upon connections.

For example, if we have a very simple network with input and output size of 1, then the input node has an ID of 0 and the output 1. For a node ID 2 to exist, there **must** be an enabled connection with input/output ID's of 0/2. Clearly this state would be useless, but this is not a problem with the NEAT algorithm. 

Nodes will only be created with a new unique number, like the innovation numbers. This prevents conflicts. 

Since all disjoint and excess genes are copied from the more fit parent, and new nodes are formed by breaking a current connection, it is unlikely that useless and non-functional structures will be copied, but this edge case is addressed in the feed forward function.

**Hyperparameters**

see: hyperSingleton.h

These are constants handled by a singleton. This is done via a singleton rather than globals, and this singleton also tracks innovation and node numbers

*Hyperparameters*

```
    const float mutateWeights;
    const float mutateConnections;
    const float mutateNodes;
    const float enableConnection;
    const int populationSize;
    const double speciesThreshold;
    const float weightsCoefficient;
    const float connectionsCoefficient;
    const float nodesCoefficient;
    const activationFunction function = activationFunction::steepSigmoid;
```

These are defined, **in this order**, when the singleton is created. The singleton is defined with list constructor notation, so the parameter names are useless. I'm so so sorry for anyone else reading that and wondering. Only the activation function (parameter `j`) has a defined default.