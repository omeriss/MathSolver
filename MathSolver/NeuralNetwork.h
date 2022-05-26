#pragma once
#include <vector>
#include <functional>
#include "Layer.h"

NetworkType* disCostDerivative(std::vector<NetworkType> output, int label);

class NeuralNetwork
{
public:
	/// <summary>
	/// constructor
	/// </summary>
	/// <param name="layers"> vector of layers</param>
	/// <param name="inputSize"> the size of input</param>
	/// <param name="outputSize"> the size of output</param>
	/// <param name="path"> the path to load layers from</param>
	NeuralNetwork(std::vector<Layer*> layers, size_t inputSize, size_t outputSize, std::string path = "");

	/// <summary>
	/// the whole feed forward
	/// </summary>
	/// <param name="input"> input layer</param>
	/// <returns> output layer</returns>
	std::vector<NetworkType> FeedForword(NetworkType* input);

	/// <summary>
	/// the whole backprop process
	/// </summary>
	/// <param name="input"> input</param>
	/// <param name="label"> true lable</param>
	/// <param name="costDerivative"> the cost d function</param>
	void Backpropagation(NetworkType* input, int label, std::function<NetworkType* (std::vector<NetworkType>, int)>& costDerivative);

	/// <summary>
	/// learn from training data
	/// </summary>
	/// <param name="input"> trainig inputs</param>
	/// <param name="label"> training labels</param>
	/// <param name="costDerivative"> cost derivative func</param>
	/// <param name="epochs"> num of epochos </param>
	/// <param name="learnRate"> learn rate</param>
	/// <param name="setSize"> set size</param>
	/// <param name="savePath"> save the result to...</param>
	void Learn(std::vector<NetworkType*>& input, std::vector<int>& label, std::function<NetworkType* (std::vector<NetworkType>, int)> costDerivative, int epochs, double learnRate, int setSize, std::string savePath = "");
private:

	/// <summary>
	/// save 
	/// </summary>
	/// <param name="path"> the path to save</param>
	void SaveNetwork(std::string path);

	/// <summary>
	/// layers
	/// </summary>
	std::vector<Layer*> layers;

	/// <summary>
	/// input and output
	/// </summary>
	NetworkType* input, * output;
	size_t inputSize, outputSize;
};

