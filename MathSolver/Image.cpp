#include "Image.h"

Image::Image(sf::Texture& texture, sf::Vector2f pos, sf::Vector2f size):UiElement(texture, pos, size)
{
}

void Image::Print(sf::RenderWindow& window)
{
	window.draw(sprite);
}
