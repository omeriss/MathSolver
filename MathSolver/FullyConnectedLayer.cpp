#include "FullyConnectedLayer.h"

FullyConnectedLayer::FullyConnectedLayer(size_t inputSize, size_t outputSize) :Layer(nullptr, outputSize), weights(outputSize, inputSize), biases(outputSize)
{
	this->inputSize = inputSize;
	this->outputSize = outputSize;
	weights.RandomInit();
	biases.RandomInit();
	biasesGradient = nullptr;
	weightsGradient = nullptr;
}

void FullyConnectedLayer::UploadFromFile(std::ifstream& file)
{
	file.read((char*)biases.GetPointer(), sizeof(NetworkType) * outputSize);
	file.read((char*)weights.GetPointer(), sizeof(NetworkType) * outputSize * inputSize);
}

void FullyConnectedLayer::FeedForword()
{
	// create Vector of the input and the output
	Vector<NetworkType> inputVec(inputSize, input);
	Vector<NetworkType> outputVec(outputSize, output);

	// mul the weight matrix by the input vector and put the result in the output
	weights.mul(inputVec, outputVec);

	// add the biases
	outputVec += biases;
}

void FullyConnectedLayer::SaveToFile(std::ofstream& file)
{
	file.write((const char*)biases.GetPointer(), sizeof(NetworkType) * outputSize);
	file.write((const char*)weights.GetPointer(), sizeof(NetworkType) * outputSize * inputSize);
}

void FullyConnectedLayer::SetUpLearning()
{
	if (!inputGradient) {
		inputGradient = new NetworkType[inputSize];
		biasesGradient = new Vector<NetworkType>(outputSize);
		weightsGradient = new Matrix<NetworkType>(outputSize, inputSize);
		biasesGradient->Zeros();
		weightsGradient->Zeros();
	}
}

void FullyConnectedLayer::UpdateByGradient(double learnRate, int setSize)
{
	// mul by the learn rate and div by the set size
	(*biasesGradient) *= learnRate / setSize;
	(*weightsGradient) *= learnRate / setSize;

	// sub the gradient from the biases and weights
	biases -= *biasesGradient;
	weights -= *weightsGradient;

	// start over
	biasesGradient->Zeros();
	weightsGradient->Zeros();
}

NetworkType* FullyConnectedLayer::Backpropagation(NetworkType* outputGradient)
{
	// create vectors for the inputGradient, outputgradient and the input
	Vector<NetworkType> outputGradientVec(outputSize, outputGradient);
	Vector<NetworkType> inputVec(inputSize, input);
	Vector<NetworkType> inputGradientVec(inputSize, inputGradient);

	// error(l) -> outputgradient, error(l-1) -> inputgradient
	// dC/dW(l) = error(l) * transpose(a(l-1))
	outputGradientVec.mulTranspose(inputVec, *weightsGradient, true);

	// dC/db(l) = error(l)
	(*biasesGradient) += outputGradientVec;

	// error(l-1) = transpose(w(l)) * error(l)
	weights.TransposeMul(outputGradientVec, inputGradientVec);

	return inputGradient;
}

FullyConnectedLayer::~FullyConnectedLayer()
{
	if (biasesGradient)
		delete biasesGradient;
	if (weightsGradient)
		delete weightsGradient;
}
