#pragma once
#include "UiElement.h"
#include <vector>
#include <array>
#include <stack>
#include "Client.h"
#include "NeuralNetwork.h"

using std::vector, std::array, std::queue;

class DrawScreen :
    public UiElement
{
public:
	DrawScreen(sf::Vector2f pos, sf::Vector2f size);
	void Update(sf::RenderWindow& window);
	void Print(sf::RenderWindow& window);
	void Draw(sf::RenderWindow& window);
	void DrawAround(int x, int y);
	void DrawAndSend(int x, int y);
	void DrawLine(sf::Vector2f pos);
	void SetCell(int x, int y, sf::Color color);
	void SetLine(int y, array<sf::Color, BASE_DRAW_SCREEN_W>& pixels);
	void SetClinet(Client* cl);
	void SetColor(sf::Color color);
	void SetRadius(float r);
	void SetDrawMode(bool canDraw);
	vector<array<sf::Color, BASE_DRAW_SCREEN_W>>& GetBoard();
	std::pair<sf::IntRect, double*> findChar(int startX, int startY, sf::IntRect area, bool* done);
	vector<std::pair<sf::IntRect, double*>> FindChars(sf::IntRect area);

private:
	bool canDraw;
	NeuralNetwork* network;
	sf::RenderTexture board;
	sf::RenderTexture overlay;
	sf::Sprite OverlaySprite;
	sf::View View;
	bool lastPrassed;
	sf::Vector2f lastPos;
	bool drawingRect;
	sf::Vector2f rectStart;
	sf::RectangleShape pixel;
	vector<array<sf::Color, BASE_DRAW_SCREEN_W>> boardPixels;
	Client* clinet;
	float drawRadius;
};

