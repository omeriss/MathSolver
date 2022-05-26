#include "NeuralNetwork.h"

NeuralNetwork::NeuralNetwork(std::vector<Layer*> layers, size_t inputSize, size_t outputSize, std::string path)
{
	this->inputSize = inputSize;
	this->outputSize = outputSize;
	size_t layerCount = layers.size();
	this->input = new NetworkType[inputSize];
	layers[0]->SetInput(this->input);
	this->output = layers[layerCount - 1]->GetOutput();

	for (size_t i = 1; i < layerCount; i++) {
		layers[i]->SetInput(layers[i - 1]->GetOutput());
	}
	this->layers = layers;

	if (path != "") {
		std::ifstream file(path, std::ofstream::binary);
		if (!file) {
			std::cout << "cant open file" << std::endl;
		}
		else {
			for (auto& layer : this->layers) {
				layer->UploadFromFile(file);
			}
			file.close();
		}
	}
}

std::vector<NetworkType> NeuralNetwork::FeedForword(NetworkType* input)
{
	std::memcpy(this->input, input, sizeof(NetworkType) * inputSize);
	for (auto& layer : layers) {
		layer->FeedForword();
	}
	std::vector<NetworkType> outputCopy;
	outputCopy.resize(outputSize);
	std::memcpy(outputCopy.data(), output, sizeof(NetworkType) * outputSize);
	return outputCopy;
}

void NeuralNetwork::Backpropagation(NetworkType* input, int label, std::function<NetworkType* (std::vector<NetworkType>, int)>& costDerivative)
{
	auto output = this->FeedForword(input);
	NetworkType* outputGradient = costDerivative(output, label);
	NetworkType* temp = outputGradient;

	int maxn = 0;
	for (int i = 1; i < output.size(); i++) {
		if (output[i] > output[maxn])
			maxn = i;
	}
	if (output[maxn] > 0.9)
		std::cout << label << " " << maxn << "\n";

	int layerCount = layers.size();

	// backPropagate over all the layers from the last one to the first one
	for (int i = layerCount - 1; i >= 0; i--) {
		outputGradient = layers[i]->Backpropagation(outputGradient);
	}

	delete[] temp;
}

void randomizeVector(std::vector<NetworkType*>& input, std::vector<int>& label) {
	int s = input.size();
	for (int i = 0; i < s; i++) {
		int k = rand() % s;
		std::swap(input[i], input[k]);
		std::swap(label[i], label[k]);
	}
}

void NeuralNetwork::Learn(std::vector<NetworkType*>& input, std::vector<int>& label, std::function<NetworkType* (std::vector<NetworkType>, int)> costDerivative, int epochs, double learnRate, int setSize, std::string savePath)
{
	for (auto layer : layers)
		layer->SetUpLearning();
	int inputTotalLen = input.size();
	for (int t = 0; t < epochs; t++) {
		randomizeVector(input, label);
		for (int i = 0; i < inputTotalLen / setSize; i++) {
			for (int j = 0; j < setSize; j++) {
				int setIndex = setSize * i + j;
				Backpropagation(input[setIndex], label[setIndex], costDerivative);
			}
			for (auto layer : layers)
				layer->UpdateByGradient(learnRate, setSize);
		}
		SaveNetwork(savePath + std::to_string(t));
		std::cout << t << std::endl;
	}

	if (savePath != "") {
		SaveNetwork(savePath);
	}
}

void NeuralNetwork::SaveNetwork(std::string path)
{
	std::ofstream file("cool.dat", std::ofstream::binary);
	if (!file) {
		std::cout << "cant open file" << std::endl;
	}
	else {
		for (auto& layer : this->layers) {
			layer->SaveToFile(file);
		}
		file.close();
	}
}


NetworkType* disCostDerivative(std::vector<NetworkType> output, int label)
{
	int outputSize = output.size();
	NetworkType* outputGradient = new NetworkType[outputSize];
	for (int i = 0; i < outputSize; i++) {
		outputGradient[i] = output[i] - (i == label);
	}
	return outputGradient;
}