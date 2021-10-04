#include "pch.h"
#include "../NEAT/connection.h"

TEST(ConnectionCopyTest, ConnectionTest) {
	// create a new connection and copy it
	auto* c = new connection{ 1,2,0.5,1,true };
	// copy it
	auto* d = new connection{ c };
	// delete original 
	delete c;
	// make sure d works
	EXPECT_EQ(1, d->inputNode);
	EXPECT_EQ(2, d->outputNode);
	EXPECT_EQ(0.5, d->weight);
	EXPECT_EQ(1, d->innovationNumber);
	EXPECT_EQ(true, d->enabled);
}