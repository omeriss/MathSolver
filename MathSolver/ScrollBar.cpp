#include "ScrollBar.h"

ScrollBar::ScrollBar(std::function<void(float)> onChange, std::pair<float, float> fromTo, sf::Vector2f pos, sf::Vector2f size):UiElement(*UiElement::textureMap["scrollBar"], pos, size)
{
	scrollHead.setTexture(*UiElement::textureMap["scrollHead"]);
	scrollHead.setScale(size.y/(*UiElement::textureMap["scrollHead"]).getSize().y, size.y/(*UiElement::textureMap["scrollHead"]).getSize().y);
	scrollHead.setPosition(pos);
	this->onChange = onChange;
	this->fromTo = fromTo;
	isMoving = false;
}

void ScrollBar::Update(sf::RenderWindow& window)
{
	// if clicked

	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		if (InsideSprite(mousePos) || isMoving) {
			//change scroll
			scrollHead.setPosition(std::max(std::min(mousePos.x - scrollHead.getGlobalBounds().width/2, sprite.getPosition().x + sprite.getGlobalBounds().width - scrollHead.getGlobalBounds().width), sprite.getPosition().x), scrollHead.getPosition().y);
			onChange((scrollHead.getPosition().x - sprite.getPosition().x) / (sprite.getGlobalBounds().width - scrollHead.getGlobalBounds().width) * (fromTo.second - fromTo.first) + fromTo.first);
			isMoving = true;
		}
	}
	else {
		isMoving = false;
	}
}

void ScrollBar::Print(sf::RenderWindow& window)
{
	window.draw(sprite);
	window.draw(scrollHead);
}

void ScrollBar::SetOrigin(OriginType origin)
{
	std::cout << "Can't modify origin of scroll bar" << std::endl;
}
