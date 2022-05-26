#pragma once
#include <vector>
#include <stack>
#include <math.h>
#include <limits.h>
#include "UiElement.h"
using std::stack;
using std::pair;

/// <summary>
/// calc element is a num or a operation or x
/// </summary>
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
	/// <summary>
	/// constructor
	/// </summary>
	/// <param name="pos"> the pos of the </param>
	/// <param name="size"></param>
	/// <param name="Fx"></param>
	Graph(sf::Vector2f pos, sf::Vector2f size, std::vector<CalcElement>& Fx);

	/// <summary>
	/// update the window
	/// </summary>
	/// <param name="window"> the window</param>
	void Update(sf::RenderWindow& window);

	/// <summary>
	/// draw the graph
	/// </summary>
	void ReDrawGraph();

	/// <summary>
	/// draw on the screen
	/// </summary>
	/// <param name="window"> the window</param>
	void Print(sf::RenderWindow& window);

	/// <summary>
	/// move in the graph
	/// </summary>
	/// <param name="window"> the window</param>
	void MoveGraph(sf::RenderWindow& window);
	
	/// <summary>
	/// set new function to the graph
	/// </summary>
	/// <param name="Fx">the function</param>
	void SetFx(std::vector<CalcElement> Fx);
private:
	/// <summary>
	/// the changes of hight and with to make line more than 1 pix
	/// </summary>
	const static pair<float, float> changes[12];

	/// <summary>
	/// calc the f(x)
	/// </summary>
	/// <param name="x"> x value </param>
	/// <returns></returns>
	pair<float, bool> Calc(float x);

	/// <summary>
	/// the render tex to draw the graph on
	/// </summary>
	sf::RenderTexture renderTex;

	/// <summary>
	/// f(x)
	/// </summary>
	std::vector<CalcElement> Fx;

	/// <summary>
	/// the view of the graph
	/// </summary>
	sf::View GraphView;

	/// <summary>
	/// the weight of x
	/// </summary>
	float xWeight;

	/// <summary>
	/// is the graph moving
	/// </summary>
	bool moving;

	/// <summary>
	/// is the mouse clicked
	/// </summary>
	bool clicked;

	/// <summary>
	/// the last pos of the mouse
	/// </summary>
	sf::Vector2f lastPos;

	/// <summary>
	/// sould the graph update next frame
	/// </summary>
	bool UpdateNextFrame;
};

