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
	EquationAnalizer();
	void LoadFromImages(std::vector<std::pair<sf::IntRect, double*>> images);
	void LoadFromString(std::string equation);
	void SetSubScreen(Screen* subScreen);
private:
	void SwapScreens(bool toEdit);
	Screen* subScreen;
	std::string leftSide, rightSide;
	NeuralNetwork* network;
	std::vector<UiElement*> otherUiElements;
	bool onEdit;
};

