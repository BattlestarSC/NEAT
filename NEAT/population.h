#pragma once
#include <vector>
#include "species.h"
#include "individual.h"
#include "node.h"
#include "connection.h"

class population
{

public:
	std::vector<species*> sv;
	std::vector<individual*> individuals;
	
	struct hyperParameters hp;

};

