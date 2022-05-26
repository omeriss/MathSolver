#include "Layer.h"

Layer::Layer(NetworkType* input, NetworkType* output)
{
    this->input = input;
    this->output = output;
    inputGradient = nullptr;
}

Layer::Layer(NetworkType* input, size_t outputSize)
{
    this->input = input;
    this->output = new NetworkType[outputSize];
    inputGradient = nullptr;
}

void Layer::SetInput(NetworkType* input)
{
    this->input = input;
}

NetworkType* Layer::GetOutput()
{
    return output;
}

Layer::~Layer()
{
    delete output;
    if (inputGradient)
        delete inputGradient;
}