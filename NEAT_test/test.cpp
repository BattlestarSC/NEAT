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