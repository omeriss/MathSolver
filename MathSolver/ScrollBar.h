#pragma once
#include <functional>
#include <iostream>
#include "UiElement.h"

class ScrollBar :
    public UiElement
{
public:
    /// <summary>
    /// constructor
    /// </summary>
    /// <param name="onChange"></param>
    /// <param name="fromTo"></param>
    /// <param name="pos"></param>
    /// <param name="size"></param>
    ScrollBar(std::function<void(float)> onChange, std::pair<float, float> fromTo, sf::Vector2f pos = { 0,0 }, sf::Vector2f size = { 200, 50 });

    /// <summary>
    /// update scroll
    /// </summary>
    /// <param name="window"></param>
    void Update(sf::RenderWindow& window);
    void Print(sf::RenderWindow& window);
    void SetOrigin(OriginType origin);
private:
    bool isMoving;
    sf::Sprite scrollHead;

    /// <summary>
    /// function on change
    /// </summary>
    std::function<void(float)> onChange;

    /// <summary>
    /// start and end val
    /// </summary>
    std::pair<float, float> fromTo;
};

