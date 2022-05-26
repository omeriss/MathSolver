#include "ConvolutionalLayer.h"

/// <summary>
/// free a vector of matrices
/// </summary>
/// <param name="vec"> the vector of matrices</param>
void FreeVector(std::vector<Matrix<NetworkType>*> vec) {
	int inputSize = vec.size();
	for (int i = 0; i < inputSize; i++) {
		delete vec[i];
	}
}

/// <summary>
/// free a vector of vectors of matrices
/// </summary>
/// <param name="vec"> hte vector of vetors of matrices</param>
void FreeVectorVector(std::vector<std::vector<Matrix<NetworkType>*>> vec) {
	int inputSize1 = vec.size();
	for (int i = 0; i < inputSize1; i++) {
		int inputSize2 = vec[inputSize1].size();
		for (int j = 0; j < inputSize2; j++) {
			delete vec[inputSize1][inputSize2];
		}
	}
}

ConvolutionalLayer::ConvolutionalLayer(size_t intputWidth, size_t inputHeight, size_t channels, size_t kernelSize, size_t kernelCount)
	:Layer(nullptr, kernelCount* (inputHeight - kernelSize + 1)* (intputWidth - kernelSize + 1))
{
	// init class sizes
	this->intputWidth = intputWidth;
	this->inputHeight = inputHeight;
	this->channels = channels;
	this->kernelSize = kernelSize;
	this->kernelCount = kernelCount;
	this->outputWidth = intputWidth - kernelSize + 1;
	this->outputHeight = inputHeight - kernelSize + 1;

	// init kernels and biases
	kernels.resize(kernelCount);
	for (auto& kernelsI : kernels) {
		kernelsI.resize(channels);
		for (auto& kernel : kernelsI) {
			kernel = new Matrix<NetworkType>(kernelSize, kernelSize);
			kernel->RandomInit();
		}
	}
	biases.resize(kernelCount);
	for (auto& biasI : biases) {
		biasI = new Matrix<NetworkType>(outputHeight, outputWidth);
		biasI->RandomInit();
	}

	// init output matrices
	NetworkType* outputTemp = this->output;
	for (int i = 0; i < kernelCount; i++) {
		outputMatrices.push_back(new Matrix<NetworkType>(outputWidth, outputHeight, outputTemp));
		outputTemp = outputTemp + (outputWidth * outputHeight);
	}

}

void ConvolutionalLayer::UploadFromFile(std::ifstream& file)
{
	// upload biases
	for (auto& bias : biases) {
		file.read((char*)(*bias).GetPointer(), sizeof(NetworkType) * outputHeight * outputWidth);
	}

	// upload kernels
	for (auto& kernalvec : kernels) {
		for (auto& kernel : kernalvec) {
			file.read((char*)(*kernel).GetPointer(), sizeof(NetworkType) * kernelSize * kernelSize);
		}
	}
}

void ConvolutionalLayer::SaveToFile(std::ofstream& file)
{
	// save biases
	for (auto& bias : biases) {
		file.write((const char*)(*bias).GetPointer(), sizeof(NetworkType) * outputHeight * outputWidth);
	}

	// save kernels
	for (auto& kernalvec : kernels) {
		for (auto& kernel : kernalvec) {
			file.write((const char*)(*kernel).GetPointer(), sizeof(NetworkType) * kernelSize * kernelSize);
		}
	}
}

void ConvolutionalLayer::SetInput(NetworkType* input)
{
	// throw error if done more than once
	if (inputMatrices.size() != 0)
		throw "can't set input twice";

	// init input
	Layer::SetInput(input);
	for (int i = 0; i < channels; i++) {
		inputMatrices.push_back(new Matrix<NetworkType>(inputHeight, intputWidth, input));
		input = input + (inputHeight * intputWidth);
	}
}

void ConvolutionalLayer::SetUpLearning()
{
	// can't be done more than once
	if (kernelsGradient.size() == 0) {
		inputGradient = new NetworkType[intputWidth * inputHeight * channels];

		// init gradient kernels and biases
		kernelsGradient.resize(kernelCount);
		for (auto& kernelsG : kernelsGradient) {
			kernelsG.resize(channels);
			for (auto& kernel : kernelsG) {
				kernel = new Matrix<NetworkType>(kernelSize, kernelSize);
				kernel->Zeros();
			}
		}
		biasesGradient.resize(kernelCount);
		for (auto& biasG : biasesGradient) {
			biasG = new Matrix<NetworkType>(outputHeight, outputWidth);
			biasG->Zeros();
		}
	}
}

void ConvolutionalLayer::UpdateByGradient(double learnRate, int setSize)
{
	// sub the kernel gradient
	int i = 0;
	for (auto& kernalvec : kernels) {
		int j = 0;
		for (auto& kernel : kernalvec) {
			// mul by the learn rate and div by the set size
			(*(kernelsGradient[i][j])) *= learnRate / setSize;
			(*kernel) -= *(kernelsGradient[i][j]);
			kernelsGradient[i][j]->Zeros();
			j++;
		}
		i++;
	}

	// sub the biases gradient
	i = 0;
	for (auto& bias : biases) {
		// mul by the learn rate and div by the set size
		(*(biasesGradient[i])) *= learnRate / setSize;
		(*bias) -= *(biasesGradient[i]);
		biasesGradient[i]->Zeros();
		i++;
	}
}

void ConvolutionalLayer::FeedForword()
{
	// set the init val by the biases
	int i = 0;
	for (auto& outputMatrix : outputMatrices) {
		outputMatrix->Set(*(biases[i]));
	}

	// calc the output with the kernels
	i = 0;
	for (auto& outputMatrix : outputMatrices) {
		int j = 0;
		for (auto& inputMatrix : inputMatrices) {
			// do cress correlation
			// Y(i) = b(i) + sum for all j(X(j) cross correlate K(i,j))
			inputMatrix->CrossCorrelation(*(kernels[i][j]), *outputMatrix, true);
			j++;
		}
		i++;
	}
}

NetworkType* ConvolutionalLayer::Backpropagation(NetworkType* outputGradient)
{
	// init output Gradient matrices
	std::vector<Matrix<NetworkType>*> outputGradientMatrices;
	for (int i = 0; i < kernelCount; i++) {
		outputGradientMatrices.push_back(new Matrix<NetworkType>(outputHeight, outputWidth, outputGradient));
		outputGradient = outputGradient + (outputWidth * outputHeight);
	}

	// init input Gradient matrices
	NetworkType* inputGTemp = this->inputGradient;
	std::vector<Matrix<NetworkType>*> inputGradientMatrices;
	for (int i = 0; i < channels; i++) {
		inputGradientMatrices.push_back(new Matrix<NetworkType>(inputHeight, intputWidth, inputGTemp));
		inputGradientMatrices[inputGradientMatrices.size() - 1]->Zeros();
		inputGTemp = inputGTemp + (inputHeight * intputWidth);
	}

	// calc the kernel gradient and input gradient
	for (int i = 0; i < kernelCount; i++) {
		for (int j = 0; j < channels; j++) {
			// dC/dK(i,j) = X(j) cross correlation (dC/dY(i))
			inputMatrices[j]->CrossCorrelation(*(outputGradientMatrices[i]), *(kernelsGradient[i][j]), true);

			// dC/dX(j) = sum over all i((dC/dY(i)) Full Convolution K(i,j))
			outputGradientMatrices[i]->FullConvolution(*(kernels[i][j]), *(inputGradientMatrices[j]), true);
		}
	}

	// calc the bias gradient
	for (int i = 0; i < kernelCount; i++) {
		// dC/dB(i) = dC/dY(i)
		(*(biasesGradient[i])) += (*(outputGradientMatrices[i]));
	}

	return inputGradient;
}

ConvolutionalLayer::~ConvolutionalLayer()
{
	// free everything that was alloced
	FreeVector(inputMatrices);
	FreeVector(biases);
	FreeVector(biasesGradient);
	FreeVectorVector(kernels);
	FreeVectorVector(kernelsGradient);
}
