#pragma once
class connection
{
public:
	// Class data
	// we don't make this private, since there is essentially no need to properly encapsulate it
	unsigned long long int input;

	unsigned long long int output;

	double weight;

	unsigned long long int innovation;

	bool enabled;

	bool memory;

	// C++ list constructor, for ease
	connection(unsigned long long int in, unsigned long long int out, double w, unsigned long long int inno, bool e, bool m) : input(in), output(out), weight(w), innovation(inno), enabled(e), memory(m) {};

};

