#pragma once
#include "UiElement.h"
#include <vector>
#include <array>
#include "Client.h"

using std::vector, std::array, std::queue;

class DrawScreen :
    public UiElement
{
public:
	DrawScreen(sf::Vector2f pos, sf::Vector2f size);
	void Update(sf::RenderWindow& window);
	void Print(sf::RenderWindow& window);
	void Draw(sf::RenderWindow& window);
	void DrawLine(sf::Vector2f pos);
	void SetCell(int x, int y, sf::Color color);
	void SetLine(int y, array<sf::Color, BASE_DRAW_SCREEN_W>& pixels);
	void SetClinet(Client* cl);
	vector<array<sf::Color, BASE_DRAW_SCREEN_W>>& GetBoard();

private:
	sf::RenderTexture board;
	sf::View View;
	bool lastPrassed;
	sf::Vector2f lastPos;
	sf::RectangleShape pixel;
	vector<array<sf::Color, BASE_DRAW_SCREEN_W>> boardPixels;
	Client* clinet;
};

