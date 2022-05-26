#include "Button.h"

Button::Button(sf::Texture& texture, std::function<void()> onClick, sf::Vector2f pos, sf::Vector2f size):UiElement(texture, pos, size) {
	// set the params of the button
	text.setOrigin(text.getGlobalBounds().width / 2, text.getGlobalBounds().height / 2);
	text.setPosition(pos.x + sprite.getGlobalBounds().width / 2, pos.y + sprite.getGlobalBounds().height / 2.5);
	this->onClick = onClick;
	isClicked = false;
}

void Button::SetString(sf::String str, sf::Font& font, sf::Color textColor, float textSize)
{
	// set the text of the button
	text.setFont(font);
	text.setFillColor(textColor);
	text.setCharacterSize(textSize);
	text.setString(str);
}

void Button::Update(sf::RenderWindow& window)
{
	// check if button is clicked
	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	if (InsideSprite(mousePos)) {
		sprite.setColor(sf::Color(150, 150, 150));
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			if (!isClicked && !disabled) {
				// if the button is clicked call oncilck
				onClick();
			}
			isClicked = true;
		}
		else {
			isClicked = false;
		}
	}
	else {
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			isClicked = true;
		else
			isClicked = false;
		sprite.setColor(sf::Color(255, 255, 255));
	}

}

void Button::Print(sf::RenderWindow& window)
{
	// print the button
	window.draw(sprite);
	window.draw(text);
}

void Button::SetOrigin(OriginType origin)
{
	// set the origin of the button
	sf::Vector2f oldOrigin = this->sprite.getOrigin();
	UiElement::SetOrigin(origin);
	text.setPosition(text.getPosition().x - (sprite.getOrigin().x - oldOrigin.x)* sprite.getScale().x, text.getPosition().y - (sprite.getOrigin().y - oldOrigin.y)* sprite.getScale().y);
}
