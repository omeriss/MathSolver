#include "DragBar.h"

DragBar::DragBar(sf::Texture& texture, Screen* screen, sf::Vector2f pos, sf::Vector2f size) :UiElement(texture, pos, size)
{
	this->screen = screen;
}

void DragBar::Update(sf::RenderWindow& window)
{
	// check if drag bar is clicked
	sf::Vector2i mousePosScreen = sf::Mouse::getPosition(window);
	sf::Vector2f mousePos = window.mapPixelToCoords(mousePosScreen);
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)
		&& (InsideSprite(mousePos) || isMoving)) {
		if (isMoving) {
			// if clicked move the screen
			sf::Vector2f screenCords = {
				screen->view.getViewport().left * window.getSize().x + (mousePosScreen.x - lastPos.x),
				screen->view.getViewport().top * window.getSize().y + (mousePosScreen.y - lastPos.y) };
			screen->view.setViewport({ screenCords.x / window.getSize().x , screenCords.y / window.getSize().y, screen->view.getViewport().width, screen->view.getViewport().height });
		}
		isMoving = true;
		lastPos = mousePosScreen;
	}
	else
		isMoving = false;
}

void DragBar::Print(sf::RenderWindow& window)
{
	// draw the dragbar
	window.draw(sprite);
}

