#pragma once
#include "Layer.h"

class ActivationFunction :
	public Layer
{
public:
	/// <summary>
	/// constructor
	/// </summary>
	/// <param name="layerSize"> the size of the layer/param>
	ActivationFunction(size_t layerSize);

	/// <summary>
	/// feed Forward of activation
	/// </summary>
	void FeedForword();

	/// <summary>
	/// save to file - ignored
	/// </summary>
	/// <param name="file"> the file ofstream</param>
	void SaveToFile(std::ofstream& file) {};

	/// <summary>
	/// upload from file - ignored
	/// </summary>
	/// <param name="file"> the file ifstream</param>
	void UploadFromFile(std::ifstream& file) {};

	/// <summary>
	/// update by gradient - ignored
	/// </summary>
	/// <param name="learnRate"> learn rate</param>
	/// <param name="setSize"> the set size </param>
	void UpdateByGradient(double learnRate, int setSize) {};

	/// <summary>
	/// set up the learning for the layer
	/// </summary>
	void SetUpLearning();

	/// <summary>
	/// backpropegation
	/// </summary>
	/// <param name="outputGradient"> the output gradient</param>
	/// <returns> the input gradient </returns>
	NetworkType* Backpropagation(NetworkType* outputGradient);

	/// <summary>
	/// the activation func
	/// </summary>
	/// <param name="x"> the func calcs f(x), this is the x</param>
	/// <returns></returns>
	virtual NetworkType Function(NetworkType x) = 0;

	/// <summary>
	/// the avtivatoin functoin derivative
	/// </summary>
	/// <param name="x">the func calcs f'(x), this is the x</param>
	/// <returns>the f'(x)</returns>
	virtual NetworkType FunctionDerivative(NetworkType x) = 0;
private:
	size_t layerSize;
};


class Sigmoid :
	public ActivationFunction
{
public:
	/// <summary>
	/// constructor
	/// </summary>
	/// <param name="layerSize"> the size of the layer</param>
	Sigmoid(size_t layerSize);

	/// <summary>
	/// sigmoid function
	/// </summary>
	/// <param name="x"> x </param>
	/// <returns> sig(x) </returns>
	NetworkType Function(NetworkType x);

	/// <summary>
	/// sigmoid derivative
	/// </summary>
	/// <param name="x"> x </param>
	/// <returns> sigDerivative(x) </returns>
	NetworkType FunctionDerivative(NetworkType x);
};