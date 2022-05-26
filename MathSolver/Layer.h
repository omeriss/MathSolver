#pragma once
#include <string>
#include <fstream>
#include "Matrix.h"

typedef double NetworkType;

class Layer
{
public:
	/// <summary>
	/// constructor
	/// </summary>
	/// <param name="input">input</param>
	/// <param name="output">output</param>
	Layer(NetworkType* input, NetworkType* output);

	/// <summary>
	/// constructor
	/// </summary>
	/// <param name="input">input</param>
	/// <param name="outputSize">o size</param>
	Layer(NetworkType* input, size_t outputSize);

	/// <summary>
	/// set the input
	/// </summary>
	/// <param name="input"> input</param>
	virtual void SetInput(NetworkType* input);
	NetworkType* GetOutput();

	/// <summary>
	/// virtual funcs for cnn and fully connected to implement
	/// </summary>
	virtual void FeedForword() = 0;
	virtual NetworkType* Backpropagation(NetworkType* outputGradient) = 0;
	virtual void UpdateByGradient(double learnRate, int setSize) = 0;
	virtual void SaveToFile(std::ofstream& file) = 0;
	virtual void UploadFromFile(std::ifstream& file) = 0;
	virtual void SetUpLearning() = 0;
	~Layer();
protected:
	/// <summary>
	/// input of layer
	/// </summary>
	NetworkType* input;

	/// <summary>
	/// output of layer
	/// </summary>
	NetworkType* output;

	/// <summary>
	/// input gradient of layer
	/// </summary>
	NetworkType* inputGradient;
};