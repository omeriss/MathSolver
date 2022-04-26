#pragma once
#include "UiElement.h"
#include <functional>
class Button :
    public UiElement
{
public:
    Button(sf::Texture& texture, std::function<void()> onClick, sf::Vector2f pos = { 0,0 }, sf::Vector2f size = { 200, 50 });
    void SetString(sf::String str, sf::Font& font, sf::Color textColor = sf::Color::Black, float textSize = 30);
    void Update(sf::RenderWindow& window);
    void Print(sf::RenderWindow& window);
    void SetOrigin(OriginType origin);
private:
    sf::Text text;
    std::function<void()> onClick;
    bool isClicked;
    std::atomic_bool disabled;
};

