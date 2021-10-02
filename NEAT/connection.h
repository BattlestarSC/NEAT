#pragma once

/*
* We want to represent a connection between two nodes
*/
class connection
{
	// important details
public:
	unsigned long long int inputNode;
	unsigned long long int outputNode;
	double weight;
	unsigned long long int innovationNumber;
	bool enabled;

	/// <summary>
	/// Create a connection based on full provided details
	/// </summary>
	/// <param name="in">input node ID</param>
	/// <param name="out">output node ID</param>
	/// <param name="w">connection weight</param>
	/// <param name="innovation">innovation number</param>
	/// <param name="e">enable bit</param>
	connection(unsigned long long int in, unsigned long long int out, double w, unsigned long long int innovation, bool e) : inputNode(in), outputNode(out), weight(w), innovationNumber(innovation), enabled(e) {};
	/// <summary>
	/// New connection with a random weight and enabled
	/// </summary>
	/// <param name="in">input node ID</param>
	/// <param name="out">output node ID</param>
	/// <param name="innovation">innovation number (must be assigned)</param>
	connection(unsigned long long int in, unsigned long long int out, unsigned long long int innovation);
	/// <summary>
	/// Copy constructor
	/// </summary>
	/// <param name="cpy">connection to copy</param>
	connection(connection* cpy);

	/// <summary>
	/// Just make life easy for execution
	/// Multiply value by weight and return, nothing else
	/// </summary>
	/// <param name="value">what the connection senses</param>
	/// <returns></returns>
	double input(double value);
};

