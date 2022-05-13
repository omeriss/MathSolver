#pragma once
#include "UiElement.h"
class Image :
    public UiElement
{
public:
	Image(sf::Texture& texture, sf::Vector2f pos, sf::Vector2f size);
	void Update(sf::RenderWindow& window) {};
	void Print(sf::RenderWindow& window);
	void ChangeTexture(sf::Texture& texture);
};

