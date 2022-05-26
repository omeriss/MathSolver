#pragma once
#include "UiElement.h"
#include <vector>
#include <array>
#include <stack>
#include "Client.h"
#include "EquationAnalizer.h"

using std::vector, std::array, std::queue;

class DrawScreen :
    public UiElement
{
public:
	/// <summary>
	/// constructor
	/// </summary>
	/// <param name="pos">the pos of the drawscreen</param>
	/// <param name="size">the size</param>
	/// <param name="subScreen"> the subscreen for the function</param>
	DrawScreen(sf::Vector2f pos, sf::Vector2f size, Screen* subScreen = nullptr);
	/// <summary>
	/// update the drawscreen
	/// </summary>
	/// <param name="window">the window</param>
	void Update(sf::RenderWindow& window);
	/// <summary>
	/// print the drawscreen
	/// </summary>
	/// <param name="window">the window</param>
	void Print(sf::RenderWindow& window);
	/// <summary>
	/// draw of the screen
	/// </summary>
	/// <param name="window">the window</param>
	void Draw(sf::RenderWindow& window);
	/// <summary>
	/// paint pixels around pixel
	/// </summary>
	/// <param name="x"> the x</param>
	/// <param name="y">the y</param>
	void DrawAround(int x, int y);
	/// <summary>
	/// paint the pixel and send to server
	/// </summary>
	/// <param name="x">the x of the pixel</param>
	/// <param name="y"> the y of the pixel</param>
	void DrawAndSend(int x, int y);
	/// <summary>
	/// draw a line from 2 points
	/// </summary>
	/// <param name="pos"> the end point</param>
	void DrawLine(sf::Vector2f pos);
	/// <summary>
	/// set a cell to color
	/// </summary>
	/// <param name="x">x</param>
	/// <param name="y">y</param>
	/// <param name="color">the color</param>
	void SetCell(int x, int y, sf::Color color);
	/// <summary>
	/// set a line to color
	/// </summary>
	/// <param name="y">y of the line</param>
	/// <param name="pixels">the color</param>
	void SetLine(int y, array<sf::Color, BASE_DRAW_SCREEN_W>& pixels);
	/// <summary>
	/// set the client object
	/// </summary>
	/// <param name="cl">the client</param>
	void SetClinet(Client* cl);
	/// <summary>
	/// set pen color
	/// </summary>
	/// <param name="color"> the color </param>
	void SetColor(sf::Color color);
	/// <summary>
	/// set the r of the pen
	/// </summary>
	/// <param name="r"> the radius</param>
	void SetRadius(float r);
	/// <summary>
	/// set if can draw
	/// </summary>
	/// <param name="canDraw">can the user draw</param>
	void SetDrawMode(bool canDraw);
	/// <summary>
	/// clear the screen
	/// </summary>
	void Clear();
	/// <summary>
	/// set the sub screen
	/// </summary>
	/// <param name="subScreen">the sub screen</param>
	void SetSubScreen(Screen* subScreen);
	/// <summary>
	/// get the board
	/// </summary>
	/// <returns>the board</returns>
	vector<array<sf::Color, BASE_DRAW_SCREEN_W>>& GetBoard();
	/// <summary>
	/// find a char on the board
	/// </summary>
	/// <param name="startX"> the start position x</param>
	/// <param name="startY"> the start position y</param>
	/// <param name="area"> the area to search in</param>
	/// <param name="done"> arr is the cell was already searched </param>
	/// <returns></returns>
	std::pair<sf::IntRect, double*> findChar(int startX, int startY, sf::IntRect area, bool* done);
	/// <summary>
	/// find all the chars that were marked
	/// </summary>
	/// <param name="area"> the area to search in</param>
	/// <returns></returns>
	vector<std::pair<sf::IntRect, double*>> FindChars(sf::IntRect area);

private:
	/// <summary>
	/// can the use draw
	/// </summary>
	bool canDraw;
	/// <summary>
	/// render tex of the board
	/// </summary>
	sf::RenderTexture board;
	/// <summary>
	/// overlay to print the marking rect
	/// </summary>
	sf::RenderTexture overlay;
	/// <summary>
	/// sprite for the overlay
	/// </summary>
	sf::Sprite OverlaySprite;
	/// <summary>
	/// the view of the board
	/// </summary>
	sf::View View;
	/// <summary>
	/// was the mouse pressed last frame
	/// </summary>
	bool lastPrassed;
	/// <summary>
	/// the last pos of the mouse
	/// </summary>
	sf::Vector2f lastPos;
	/// <summary>
	/// is it drawing rect
	/// </summary>
	bool drawingRect;
	/// <summary>
	/// the start pos of the mark rect
	/// </summary>
	sf::Vector2f rectStart;
	/// <summary>
	/// one pixel, keeps the color of the pen as well
	/// </summary>
	sf::RectangleShape pixel;
	/// <summary>
	/// the pixels of the board
	/// </summary>
	vector<array<sf::Color, BASE_DRAW_SCREEN_W>> boardPixels;
	/// <summary>
	/// poninter to the client
	/// </summary>
	Client* clinet;
	/// <summary>
	/// the r of the pen
	/// </summary>
	float drawRadius;
	/// <summary>
	/// the class to solve the equations
	/// </summary>
	EquationAnalizer equationAnalizer;
};

