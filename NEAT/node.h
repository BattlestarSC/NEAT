#pragma once
#include <vector>
#include "connection.h"
class node
{
public:
	unsigned long long int ID;
	std::vector<connection*> connections;
	double sense{ 0.0 };
	double activation();

public:
	node(unsigned long long int id) :ID(id){};
	node(node* cpy);
	void addConnection(connection* add);
	double getOutput();
	void feedConnection(unsigned long long int inputID, double input);
	int getEnabledConnectionsCount();

	~node();
};

