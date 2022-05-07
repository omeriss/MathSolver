#include "ToggleButton.h"

ToggleButton::ToggleButton(sf::Texture& texture1, sf::Texture& texture2, std::function<void(bool)> onClick, sf::Vector2f pos, sf::Vector2f size) :
	UiElement(texture1, pos, size), texture1(texture1), texture2(texture2)
{
	this->onClick = onClick;
	isClicked = false;
	state = false;
	buttonMode = true;
}

void ToggleButton::Update(sf::RenderWindow& window)
{
	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	if (InsideSprite(mousePos) && buttonMode) {
		sprite.setColor(sf::Color(150, 150, 150));
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			if (!isClicked && !disabled) {
				ChangeState(!state);
				onClick(state);
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

void ToggleButton::Print(sf::RenderWindow& window)
{
	window.draw(sprite);
}

void ToggleButton::SetButtonMode(bool buttonMode)
{
	this->buttonMode = buttonMode;
}

void ToggleButton::ChangeState(bool state)
{
	this->state = state;
	if (state) {
		sprite.setTexture(texture2);
	}
	else {
		sprite.setTexture(texture1);
	}
}
