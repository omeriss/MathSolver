#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>
#include <regex>
#include <map>
#include "Screen.h"
#include "NeuralNetwork.h"
#include "ConvolutionalLayer.h"
#include "FullyConnectedLayer.h"
#include "ActivationFunction.h"
#include "Graph.h"
#include "DragBar.h"
#include "Image.h"
#include "Button.h"
#include "TextBox.h"

static char symbols[] = { '0','1','2','3','4','5','6','7','8','9','+','-','x','y' };

static std::unordered_map<char, int> opRank = {
	{'+', 0},
	{'-', 0},
	{'*', 1},
	{'/', 1},
	{'^', 2}
};

static std::vector okCharsFromEqString = { '0','1','2','3','4','5','6','7','8','9','+','-', '/', '^', '*', 'x', 'y', '=', '(', ')'};

class EquationAnalizer
{
public:
	/// <summary>
	/// constructor
	/// </summary>
	EquationAnalizer();
	/// <summary>
	/// load data from images
	/// </summary>
	/// <param name="images"></param>
	void LoadFromImages(std::vector<std::pair<sf::IntRect, double*>> images);
	/// <summary>
	/// load by string
	/// </summary>
	/// <param name="equation"> the string</param>
	void LoadFromString(std::string equation);

	/// <summary>
	/// create the elements for the subscreen
	/// </summary>
	/// <param name="subScreen"> the subscreen</param>
	void SetSubScreen(Screen* subScreen);
private:
	void SwapScreens(bool toEdit);
	Screen* subScreen;
	std::string leftSide, rightSide;
	NeuralNetwork* network;
	std::vector<UiElement*> otherUiElements;
	bool onEdit;
};

