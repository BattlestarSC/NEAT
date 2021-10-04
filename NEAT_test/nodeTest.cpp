#include "pch.h"
#include "../NEAT/node.h"
#include "../NEAT/connection.h"

TEST(constructorTest, nodeTest) {
	// setup
	auto* nodeA = new node(1);

	// make sure we have no nullptrs
	EXPECT_FALSE(nodeA == nullptr);
	EXPECT_TRUE(nodeA->connections.size() == 0);
	
	// add connections
	auto* con = new connection(0, 1, 0.5, 0, true);

	// check addition
	EXPECT_EQ(con, nodeA->connections[0]);

	// check copying
	auto* nodeB = new node(nodeA);
	
	// make sure all went well
	EXPECT_TRUE(nodeA->ID == nodeB->ID);
	EXPECT_EQ(nodeA->connections.size(), nodeB->connections.size());
	EXPECT_EQ(nodeA->connections[0], nodeB->connections[0]);

	// cleanup
	delete nodeA, nodeB;
}

TEST(feedTest, nodeTest) {
	// setup a basic network
	std::vector<node*> nodes{};
	for (int i = 0; i < 10; i++) {
		nodes.push_back(new node(i));
	}
	double expectedOutput{ 0 };
	for (int i = 0; i < 9; i++) {
		nodes[9]->addConnection(new connection(i, 9, 0.5, i, true));
		nodes[i]->sense = i;
		expectedOutput += i * 0.5;
	}
	// test node
	node t = node(15);
	// use this for activation function
	t.sense = expectedOutput;
	expectedOutput = t.getOutput();

	// now test feeding
	for (int i = 0; i < 9; i++) {
		nodes[9]->feed(nodes[9]->connections[i], i);
	}

	EXPECT_EQ(expectedOutput, nodes[9]->getOutput());
}