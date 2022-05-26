#pragma once
#include "Layer.h"
class ConvolutionalLayer :
	public Layer
{
public:
	/// <summary>
	/// constructor
	/// </summary>
	/// <param name="intputWidth"> the width of the input </param>
	/// <param name="inputHeight"> hte hight of the input </param>
	/// <param name="channels"> the num of channels </param>
	/// <param name="kernelSize"> the size of a kernel</param>
	/// <param name="kernelCount"> the kernel count </param>
	ConvolutionalLayer(size_t intputWidth, size_t inputHeight, size_t channels, size_t kernelSize, size_t kernelCount);

	/// <summary>
	/// upload the biases and kernels to a file
	/// </summary>
	/// <param name="file"> file ifstream</param>
	void UploadFromFile(std::ifstream& file);

	/// <summary>
	/// set the input to the layer, init the inputmats
	/// </summary>
	/// <param name="input"> input pointer</param>
	void SetInput(NetworkType* input);

	/// <summary>
	/// feed forword for cnn
	/// </summary>
	void FeedForword();

	/// <summary>
	/// save the biases and kernels to file 
	/// </summary>
	/// <param name="file"> file ofstream</param>
	void SaveToFile(std::ofstream& file);

	/// <summary>
	/// set up the input gradient
	/// </summary>
	void SetUpLearning();

	/// <summary>
	/// update the biases and kernels by the set of backprops
	/// </summary>
	/// <param name="learnRate"> the learn rate</param>
	/// <param name="setSize"> the size of the set</param>
	virtual void UpdateByGradient(double learnRate, int setSize);

	/// <summary>
	/// backprop for cnn
	/// </summary>
	/// <param name="outputGradient"> the output gradient</param>
	/// <returns>the input gradient</returns>
	NetworkType* Backpropagation(NetworkType* outputGradient);

	~ConvolutionalLayer();
private:
	/// <summary>
	/// all the sizes
	/// </summary>
	size_t intputWidth, inputHeight, channels, kernelSize, kernelCount;
	size_t outputWidth, outputHeight;

	/// <summary>
	/// the kernels
	/// </summary>
	std::vector<std::vector<Matrix<NetworkType>*>> kernels;

	/// <summary>
	/// the biases
	/// </summary>
	std::vector<Matrix<NetworkType>*> biases;

	/// <summary>
	/// gradient
	/// </summary>
	std::vector<std::vector<Matrix<NetworkType>*>> kernelsGradient;
	std::vector<Matrix<NetworkType>*> biasesGradient;

	/// <summary>
	/// input and ouput matricis
	/// </summary>
	std::vector<Matrix<NetworkType>*> inputMatrices;
	std::vector<Matrix<NetworkType>*> outputMatrices;
};

