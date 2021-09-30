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

	//cleanup
	delete ind;

	//cleanup
	delete p;
}