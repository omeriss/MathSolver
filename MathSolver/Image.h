#pragma once
#include "UiElement.h"
class Image :
    public UiElement
{
public:
	/// <summary>
	/// constructor
	/// </summary>
	/// <param name="texture"></param>
	/// <param name="pos"></param>
	/// <param name="size"></param>
	Image(sf::Texture& texture, sf::Vector2f pos, sf::Vector2f size);

	/// <summary>
	/// update
	/// </summary>
	/// <param name="window"></param>
	void Update(sf::RenderWindow& window) {};

	/// <summary>
	/// draw on screen
	/// </summary>
	/// <param name="window"></param>
	void Print(sf::RenderWindow& window);
	void ChangeTexture(sf::Texture& texture);
};

