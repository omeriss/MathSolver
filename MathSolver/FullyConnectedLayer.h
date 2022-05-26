#pragma once
#include "Layer.h"
class FullyConnectedLayer :
	public Layer
{
public:
	/// <summary>
	/// constructor
	/// </summary>
	/// <param name="inputSize"> the size of the input</param>
	/// <param name="outputSize">the size of the output</param>
	FullyConnectedLayer(size_t inputSize, size_t outputSize);

	/// <summary>
	/// upload from file
	/// </summary>
	/// <param name="file"> file ifstream</param>
	void UploadFromFile(std::ifstream& file);

	/// <summary>
	/// feed forward
	/// </summary>
	void FeedForword();

	/// <summary>
	/// save to file the w and b
	/// </summary>
	/// <param name="file"> file ifstream</param>
	void SaveToFile(std::ofstream& file);

	/// <summary>
	/// set up the learninig
	/// </summary>
	void SetUpLearning();

	/// <summary>
	/// update b and w by grad
	/// </summary>
	/// <param name="learnRate"> the learn rate</param>
	/// <param name="setSize">size of backprop set</param>
	virtual void UpdateByGradient(double learnRate, int setSize);

	/// <summary>
	/// backprop for fully connected
	/// </summary>
	/// <param name="outputGradient">output gradient</param>
	/// <returns>input gradient</returns>
	NetworkType* Backpropagation(NetworkType* outputGradient);
	~FullyConnectedLayer();
private:
	/// <summary>
	/// sizes
	/// </summary>
	size_t inputSize, outputSize;

	/// <summary>
	/// w
	/// </summary>
	Matrix<NetworkType> weights;

	/// <summary>
	/// b
	/// </summary>
	Vector<NetworkType> biases;

	/// <summary>
	/// w and b gradients
	/// </summary>
	Matrix<NetworkType>* weightsGradient;
	Vector<NetworkType>* biasesGradient;
};

