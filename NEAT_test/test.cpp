#include "pch.h"

#include "../NEAT/individual.h"
#include "../NEAT/hyperSingleton.h"

TEST(TestCaseTEST, TestTestProgram) {
	EXPECT_EQ(1, 1);
	EXPECT_TRUE(true);
}

TEST(TestIndividualCreation, TestIndividualNew) {
	//This should do nothing but provide randomness
	auto* p = new hyperSingleton{ 0,0,0,0,0,0,0,0,0,activationFunction::relu };

	//We want to test this creation method
	auto* ind = new individual{ 5,2,p };

	//Test creation
	//First fitness should be empty
	EXPECT_DOUBLE_EQ(0.0, ind->fitness);

	//Make sure the list is non-empty
	EXPECT_TRUE((ind->connections.size() > 0));

	//Make sure the list is the correct size
	EXPECT_EQ(ind->connections.size(), 10); // for 5 > 2 fully connected network, there should be 10 connnections

	//Make sure there are no nulls in the lsit
	for (auto* c : ind->connections) {
		EXPECT_TRUE(c != nullptr);
	}

	//Make sure weights are assigned randomly
	for (auto* c : ind->connections) {
		EXPECT_FALSE(c->weight == 0.0);
	}

	//Make sure all indexes are valid
	for (auto* c : ind->connections) {
		EXPECT_TRUE(c->inputNode < 5);
	}
	for (auto* c : ind->connections) {
		EXPECT_TRUE(c->outputNode == 6 || c->outputNode == 5);
	}

	//cleanup
	delete ind;

	//cleanup
	delete p;
}

TEST(TestIndividualCreation, TestIndividualCopu) {
	//This should do nothing but provide randomness
	auto* p = new hyperSingleton{ 0,0,0,0,0,0,0,0,0,activationFunction::relu };

	//We want to test this creation method
	auto* ind = new individual{ 5,2,p };

	//And now make our lab-made child
	auto* cpy = new individual{ ind };

	// set up list iterators
	auto ind_front = ind->connections.begin();
	auto cpy_front = cpy->connections.begin();

	// check all connections but the last one
	while (ind_front != ind->connections.end() && cpy_front != cpy->connections.end()) {
		struct connection* ic = *ind_front;
		struct connection* cc = *cpy_front;
		EXPECT_TRUE(ic->innovationNumber == cc->innovationNumber);
		EXPECT_TRUE(ic->weight == cc->weight);
		EXPECT_TRUE(ic->enable == cc->enable);
		EXPECT_TRUE(ic->inputNode == cc->inputNode);
		EXPECT_TRUE(ic->outputNode == cc->outputNode);
		ind_front++;
		cpy_front++;
	}

	// make sure the length is the same
	EXPECT_TRUE(ind_front == ind->connections.end() && cpy_front == cpy->connections.end());

	//cleanup
	delete cpy;

	//cleanup
	delete ind;

	//cleanup
	delete p;
}

TEST(mutateConnectionFunctional, mutateConnection) {
	// Setup
	//This should do nothing but provide randomness
	auto* p = new hyperSingleton{ 0,0,0,0,0,0,0,0,0,activationFunction::relu };

	//We want to test this creation method
	auto* ind = new individual{ 5,2,p };
	auto innoNumber = p->innovationNumber;

	// run. Since it is fully connected, we EXPECT a failure
	EXPECT_TRUE(ind->mutateConnection(0,0) == nullptr);
	// make sure nothing is fucked
	EXPECT_EQ(innoNumber, p->innovationNumber);
	EXPECT_EQ(ind->connections.size(), 10);

	// Add a new node, code stolen from mutate function
	// pick a connection to break
	auto it = ind->connections.begin();
	std::advance(it, rand() % ind->connections.size());
	auto* con = (*it);

	// make the two new connections
	auto newNodeID = ind->parameterSingleton->getNodeNumber();
	auto* newA = ind->mutateConnection(con->inputNode, newNodeID, true, 1.0);
	auto* newB = ind->mutateConnection(newNodeID, con->outputNode, true, con->weight);

	// Test results
	// First, both should work, so expect two non-nullptr results
	EXPECT_TRUE(newA != nullptr);
	EXPECT_TRUE(newB != nullptr);

	// make sure that they have been added to the list
	EXPECT_EQ(ind->connections.size(), 12);
	bool foundA{false}, foundB{ false };
	for (auto* c : ind->connections) {
		if (c == newA) {
			foundA = true;
		}
		if (c == newB) {
			foundB = true;
		}
	}
	EXPECT_TRUE(foundA);
	EXPECT_TRUE(foundB);
	// make sure they have valid weights and innovation numbers
	EXPECT_EQ(innoNumber + 2, p->innovationNumber);
	EXPECT_TRUE(newA->innovationNumber >= innoNumber && newA->innovationNumber < p->innovationNumber);
	EXPECT_TRUE(newB->innovationNumber >= innoNumber && newB->innovationNumber < p->innovationNumber);
	EXPECT_FALSE(newA->innovationNumber == newB->innovationNumber);
	// chances of this are 0 if made correctly
	EXPECT_FALSE(newA->weight == newB->weight);
	
	// Now add a new connection to any location
	auto* newC = ind->mutateConnection(0, 0);

	// We expect a not nullptr result. Since everything else has been checked, we only need check this
	EXPECT_FALSE(newC == nullptr);

	// cleanup
	delete p;
	delete ind;
 
}