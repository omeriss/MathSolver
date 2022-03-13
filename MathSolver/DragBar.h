#pragma once
#include "UiElement.h"
#include "Screen.h"
class DragBar :
    public UiElement
{
public:
    DragBar(sf::Texture& texture, Screen* screen, sf::Vector2f pos = { 0,0 }, sf::Vector2f size = { 200, 50 });
    void Update(sf::RenderWindow& window);
    void Print(sf::RenderWindow& window);
private:
    Screen* screen;
    bool isMoving;
    sf::Vector2i lastPos;
};

