#include "pch.h"
#include "individual.h"
#include "connection.h"
#include <cctype>
#include <random>

// helper function to make life easier
// Is it stupid to reseed a new random engine EVERY TIME? Yes
//		Will I spend the effort to fix that? No
// TODO: if importing, change this randomness and that in connection.cpp
double randomDoubleInRange(float a, float b, std::mt19937 gen) {
	// C++ randomness
	// see: https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
	std::uniform_real_distribution<> dis(a, b);
	return dis(gen);
}

individual::individual(int inSize, int outSize)
{
	this->inputSize = inSize;
	this->outputSize = outSize;
	// create nodes
	for (unsigned long long int i = inSize; i < outSize; i++) {
		auto* newNode = new node(i);
		// create connections
		for (unsigned long long int j = 0; i < inSize; i++) {
			auto* con = new connection(j,
				i,
				newNode->connections.size()); // This should work due to off by one. i.e. innovation 0 will be when its empty (size = 0), 1 when there is currently 1 in there (0 in there, so size = 1), etc
			newNode->addConnection(con);
		}
		this->nodes.push_back(newNode);
	}
}

// helper function, just don't repeat copy code
void individual::copy(individual* cpy) {
	this->inputSize = cpy->inputSize;
	this->outputSize = cpy->outputSize;
	for (auto* n : cpy->nodes) {
		// copy and push
		this->nodes.push_back(new node(n));
	}
}

// just copy shit so when deallocation of previous occurs we are fine
individual::individual(individual* cpy, bool mutate)
{
	this->copy(cpy);

	if (mutate) {
		// we can decide what to do with the result later, right now just auto-discard
		this->mutate();
	}
}

individual::individual(individual* father, individual* mother, float enable, bool mutate)
{
	// So first we copy the more fit parent
	// this keeps all disjoint and excess genes from the more fit parent
	auto* moreFit = (father->fitness >= mother->fitness) ? father : mother;
	auto* lessFit = (moreFit == father) ? mother : father;

	this->copy(moreFit);
	
	// now crossover
	// go through every connection and 50% chance to take it from the less fit parent
	for (auto* n : this->nodes) {
		for (auto* c : n->connections) {
			// check chance
			if (randomDoubleInRange(0, 1, this->gen) < 0.5) {
				// steal genes
				/*
				* This method is wasteful of CPU cycles. However since we can't guarentee that all genes are shared, this is less error prone.
				*/
				for (auto* wn : lessFit->nodes) {
					bool found{ false };
					for (auto* wc : wn->connections) {
						if (wc->innovationNumber == c->innovationNumber) {
							found = true;
							c->enabled = wc->enabled;
							c->weight = wc->weight;
						}
					}
					if (found) {
						break;
					}
				}
			}
		}
	}

	// Here we check enable chances. It WOULD be more efficient to throw this into the last loop, but it is cleaner (and therefore harder to screw up) like this, at the cost of CPU cycles
	for (auto* n : this->nodes) {
		for (auto* c : n->connections) {
			// check chance
			if (!c->enabled) {
				if (randomDoubleInRange(0, 1, this->gen) < enable) {
					c->enabled = true;
				}
			}
		}
	}

	// mutate 
	if (mutate) {
		this->mutate();
	}

}

bool individual::fullyConnected() {
	// linear scan
	for (int n = 0; n < this->nodes.size(); n++) {
		// if the length of the enabled list is the same as the number of previous nodes and inputs, then it MUST be fully connected 
		// or it could contain a duplicate, but that should be a bug (on many accounts)
		// Concern: I'm worried that this method here of checking if new connections are valid combined with the layout will lead to a 
		// single really long chain of nodes. 
		if (this->nodes[n]->getEnabledConnectionsCount() != this->inputSize + n) {
			return false;
		}
	}
	return true;
}

void individual::uniformMutateWeights(double amt) {
	for (auto* n : this->nodes) {
		for (auto* c : n->connections) {
			c->weight += amt;
		}
	}
}

void individual::randomMutateWeights() {
	for (auto* n : this->nodes) {
		for (auto* c : n->connections) {
			c->weight = randomDoubleInRange(-1.5, 1.5, gen);
		}
	}
}

unsigned long long int individual::getInnovationNumber() {
	if (this->rp != nullptr) {
		auto in = this->rp->innovationNumber;
		this->rp->innovationNumber++;
		return in;
	}
	return 0;
}

unsigned long long int individual::getNodeNumber() {
	if (this->rp != nullptr) {
		auto nn = this->rp->nodeNumber;
		this->rp->nodeNumber++;
		return nn;
	}
	return 0;
}

//TODO: UNIT TEST THIS FUCKING MONSTER OF A FUNCTION
std::vector<connection*> individual::mutate(float weightProbability, float connectionProbability, float nodeProbability, float weightsUniformProbability)
{
	//output 
	std::vector<connection*> output{};

	// first handle weights, its the easiest
	if (randomDoubleInRange(0, 1, gen) <= weightProbability) {
		// decide method
		if (randomDoubleInRange(0, 1, gen) <= weightsUniformProbability) {
			this->uniformMutateWeights(randomDoubleInRange(-0.2, 0.2, gen));
		}
		else {
			this->randomMutateWeights();
		}
	}

	// now handle creating new connections
	// if probaility is true and (short circuited) we can make a new connection, do it
	if (randomDoubleInRange(0, 1, gen) < connectionProbability && !this->fullyConnected()) {

		// Find a vector of acceptable output nodes
		std::vector<unsigned long long int> validOutputs{};
		// linear scan
		for (int n = 0; n < this->nodes.size(); n++) {
			// if the length of the enabled list is the same as the number of previous nodes and inputs, then it MUST be fully connected 
			// or it could contain a duplicate, but that should be a bug (on many accounts)
			if (this->nodes[n]->getEnabledConnectionsCount() != this->inputSize + n) {
				validOutputs.push_back(n);
			}
		}

		// stupidity check
		if (validOutputs.size() != 0) {

			// pick a valid output node
			
			auto outputNodeID = validOutputs[rand() % validOutputs.size()];

			// find a vector of acceptable input nodes
			std::vector<unsigned long long int> invalidInputs{};
			for (auto* c : this->nodes[outputNodeID]->connections) {
				invalidInputs.push_back(c->inputNode);
			}
			std::vector<unsigned long long int> validInputs{};
			// search through sensors
			for (unsigned long long int i = 0; i < this->inputSize; i++) {
				// if it does not exist in invalid
				if (*std::find(invalidInputs.begin(), invalidInputs.end(), i) != i) {
					validInputs.push_back(i);
				}
			}
			// search through all prior nodes
			for (unsigned long long int i = 0; i < this->nodes.size(); i++) {
				// know when to stop
				if (this->nodes[i]->ID == outputNodeID) {
					break;
				}
				// if it does not exist in invalid
				if (*std::find(invalidInputs.begin(), invalidInputs.end(), this->nodes[i]->ID) != this->nodes[i]->ID) {
					validInputs.push_back(this->nodes[i]->ID);
				}
			}

			// pick a random input
			// and stupidity check
			if (validInputs.size() != 0) {
				auto inputNodeID = validInputs[rand() % validInputs.size()];
				// find the output node
				node* oNode = nullptr;
				for (auto* n : this->nodes) {
					if (n->ID == outputNodeID) {
						oNode = n;
						break;
					}
				}
				// now check if the connection exists but is disabled
				// and another stupidity check
				if (oNode != nullptr) {
					bool e{ false };
					for (auto* c : oNode->connections) {
						if (c->inputNode == inputNodeID) {
							c->enabled = true;
							c->weight = randomDoubleInRange(-2, 2, gen);
							e = true;
							break;
						}
					}
					if (!e) {
						auto* con = new connection(inputNodeID, outputNodeID, this->getInnovationNumber());
						oNode->addConnection(con);
						output.push_back(con);
					}
				}
			}
		}

	}

	// now create a new node
	if (randomDoubleInRange(0, 1, gen) < nodeProbability) {
		std::vector<connection*> allConnections{};
		for (auto* n : this->nodes) {
			for (auto* c : n->connections) {
				allConnections.push_back(c);
			}
		}
		// pick
		auto* broken = allConnections[rand() % allConnections.size()];
		node* originalOutputNode = nullptr;
		auto* newNode = new node{this->getNodeNumber()};
		for (auto* n : this->nodes) {
			if (n->ID == broken->outputNode) {
				originalOutputNode = n;
				break;
			}
		}
		auto* newInCon = new connection(broken->inputNode, newNode->ID, 1.0, this->getInnovationNumber(), true);
		output.push_back(newInCon);
		newNode->addConnection(newInCon);
		auto* newOutCon = new connection(newNode->ID, broken->outputNode, this->getInnovationNumber()); 
		output.push_back(newOutCon);
		originalOutputNode->addConnection(newOutCon);
		broken->enabled = false;

		// decide where to insert the new node
		int outLocation{ 0 }, inLocation{ 0 };
		for (int i = 0; i < this->nodes.size(); i++) {
			if (this->nodes[i]->ID == broken->outputNode) {
				outLocation = i;
			}
			if (this->nodes[i]->ID == broken->inputNode) {
				inLocation = i;
			}
		}
		int location = (int)std::floor(randomDoubleInRange(inLocation, outLocation, gen));
		auto it = this->nodes.begin();
		std::advance(it, location - 1);
		this->nodes.insert(it, newNode);
	}

	// now handle the new connections
	if (this->rp == nullptr) {
		for (auto* n : output) {
			this->rp->newConnections.push_back(n);
		}
	}
	return output;
}

/*
* Execute the graph, linearly
*/
std::vector<double> individual::feed(std::vector<double> sense) {
	if (sense.size() < this->inputSize) {
		// invalid sense, just die
		return std::vector<double>{};
	}
	// linear execution
	for (auto* n : this->nodes) {
		// run each connection
		for (auto* c : n->connections) {
			// check if we have a sensor input
			if (c->inputNode < this->inputSize) {
				n->feed(c, sense[c->inputNode]);
			}
			else {
				node* inNode = nullptr;
				for (auto* sn : this->nodes) {
					if (sn->ID == c->inputNode) {
						inNode = sn;
					}
				}
				n->feed(c, inNode->getOutput());
			}
		}
	}
	// since we can assume that the output nodes are last unless something is REALLY fucked
	std::vector<double> output{};
	auto startInd = this->nodes.size() - this->outputSize; // Example, if we have 5 nodes, and the last two are output, we want to start on index 3
	for (int i = startInd; i < this->nodes.size(); i++) {
		auto out = this->nodes[i]->getOutput();
		output.push_back(out);
	}
	return output;
}

// just clean dynamic memory
individual::~individual()
{
	for (auto* n : this->nodes) {
		delete n;
	}
}


