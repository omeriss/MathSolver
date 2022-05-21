#pragma once
#include <vector>
#include <stack>
#include <math.h>
#include <limits.h>
#include "UiElement.h"
using std::stack;
using std::pair;

struct CalcElement {
	union Element
	{
		float num;
		char op;
	} Element;
	bool IsNum;
};

class Graph :
    public UiElement
{
public:
	Graph(sf::Vector2f pos, sf::Vector2f size, std::vector<CalcElement>& Fx);
	void Update(sf::RenderWindow& window);
	void ReDrawGraph();
	void Print(sf::RenderWindow& window);
	void MoveGraph(sf::RenderWindow& window);
	void SetFx(std::vector<CalcElement> Fx);
private:
	const static pair<float, float> changes[12];
	pair<float, bool> Calc(float x);
	sf::RenderTexture renderTex;
	std::vector<CalcElement> Fx;
	sf::View GraphView;
	float xWeight;
	bool moving;
	bool clicked;
	sf::Vector2f lastPos;
	bool UpdateNextFrame;
};

