#include "ActivationFunction.h"

ActivationFunction::ActivationFunction(size_t layerSize) :Layer(nullptr, layerSize)
{
    this->layerSize = layerSize;
}

void ActivationFunction::FeedForword()
{
    // set the output to the result of the activation function on the input
    for (int i = 0; i < layerSize; i++) {
        output[i] = Function(input[i]);
    }
}

void ActivationFunction::SetUpLearning()
{
    if (!inputGradient)
        inputGradient = new NetworkType[layerSize];
}

NetworkType* ActivationFunction::Backpropagation(NetworkType* outputGradient)
{
    for (int i = 0; i < layerSize; i++) {
        // error(l-1) = activationderevative(a(l)) * error(l)
        inputGradient[i] = FunctionDerivative(input[i]) * outputGradient[i];
    }
    return inputGradient;
}

Sigmoid::Sigmoid(size_t layerSize) :ActivationFunction(layerSize)
{
}

NetworkType Sigmoid::Function(NetworkType x)
{
    // exp -> e^(the param)
    return (1 / (1 + exp(-x)));
}

NetworkType Sigmoid::FunctionDerivative(NetworkType x)
{
    // first calc sig
    NetworkType sigX = Function(x);

    // return the derivative
    return sigX * (1 - sigX);
}
