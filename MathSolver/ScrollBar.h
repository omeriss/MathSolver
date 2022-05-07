#pragma once
#include <functional>
#include <iostream>
#include "UiElement.h"

class ScrollBar :
    public UiElement
{
public:
    ScrollBar(std::function<void(float)> onChange, std::pair<float, float> fromTo, sf::Vector2f pos = { 0,0 }, sf::Vector2f size = { 200, 50 });
    void Update(sf::RenderWindow& window);
    void Print(sf::RenderWindow& window);
    void SetOrigin(OriginType origin);
private:
    bool isMoving;
    sf::Sprite scrollHead;
    std::function<void(float)> onChange;
    std::pair<float, float> fromTo;
};

