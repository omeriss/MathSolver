#pragma once
#include "UiElement.h"
#include <functional>
class Button :
    public UiElement
{
public:
    /// <summary>
    /// constructor
    /// </summary>
    /// <param name="texture"> the texture for the button</param>
    /// <param name="onClick"> the onclick function</param>
    /// <param name="pos"> the pos of the button</param>
    /// <param name="size"> the size of the button</param>
    Button(sf::Texture& texture, std::function<void()> onClick, sf::Vector2f pos = { 0,0 }, sf::Vector2f size = { 200, 50 });
    /// <summary>
    /// set the text of the button
    /// </summary>
    /// <param name="str"> the text</param>
    /// <param name="font"> the font</param>
    /// <param name="textColor"> text color</param>
    /// <param name="textSize">the size of the text</param>
    void SetString(sf::String str, sf::Font& font, sf::Color textColor = sf::Color::Black, float textSize = 30);
    /// <summary>
    /// update function for the button
    /// </summary>
    /// <param name="window"> the window</param>
    void Update(sf::RenderWindow& window);
    /// <summary>
    /// print function for the button
    /// </summary>
    /// <param name="window">the window</param>
    void Print(sf::RenderWindow& window);
    /// <summary>
    /// set the origin of the button
    /// </summary>
    /// <param name="origin"> the origin: center/left...</param>
    void SetOrigin(OriginType origin);
private:
    /// <summary>
    /// the text of the button
    /// </summary>
    sf::Text text;
    /// <summary>
    /// the onclick function for the button
    /// </summary>
    std::function<void()> onClick;
    /// <summary>
    /// is the mouse clicked
    /// </summary>
    bool isClicked;
    /// <summary>
    /// is the button disabled
    /// </summary>
    std::atomic_bool disabled;
};

