#pragma once
#include <vector>
#include "connection.h"
class node
{

public:
	// Node data items
	unsigned long long int id; //shortened from nodeID

	// outputs, default on creation to 0
	// Used as a summation for all inputs, so when all connections have been executed, the node output is actually activation(output)
	double output{0};
	// last output, default on creation to 0
	double previous{0};

	// All connections are from another node into this node
	// forward only connections
	std::vector<connection*> forward;
	// recursive connections
	std::vector<connection*> memory;

	// Functions
public:
	// How this node takes/processes input
	void addInput(connection* in, double value);
	// how we get the value of this node, and its historical value
	// For a memory connection into another node, we use getOutput(true), otherwise just getOutput()
	double getOutput(bool past = false);
	// reset after each network execution
	void reset();
	// Sort the connections from external class
	void sort();
	// constructor
	// We manually COPY each and every connection for easy memory management
	node(std::vector<connection*> connections, unsigned long long int ID);
	node(std::vector<connection*> connections, std::vector<connection*> mem, unsigned long long int ID); 
	// copy constructor
	node(node* cpy);
	// destructor, free all connections
	~node();

private:
	// organize connections
	// helper function for std::sort
	// see: https://www.geeksforgeeks.org/sorting-a-vector-in-c/
	//bool connectionCompare(connection* a, connection* b);
	// moved to node.cpp as non-memeber function

	// activation function
	double activation(double v);
	
};

