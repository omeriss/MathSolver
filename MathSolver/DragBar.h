#pragma once
#include "UiElement.h"
#include "Screen.h"
class DragBar :
    public UiElement
{
public:
    /// <summary>
    /// constructor
    /// </summary>
    /// <param name="texture"> the texture</param>
    /// <param name="screen">the screen of the drag bar</param>
    /// <param name="pos">the pos of the dragbar</param>
    /// <param name="size">the size</param>
    DragBar(sf::Texture& texture, Screen* screen, sf::Vector2f pos = { 0,0 }, sf::Vector2f size = { 200, 50 });
    /// <summary>
    /// update function
    /// </summary>
    /// <param name="window">the window</param>
    void Update(sf::RenderWindow& window);
    /// <summary>
    /// print function
    /// </summary>
    /// <param name="window">the window</param>
    void Print(sf::RenderWindow& window);
private:
    /// <summary>
    /// the screen the drag bar is in
    /// </summary>
    Screen* screen;
    /// <summary>
    /// is the screen moving rn
    /// </summary>
    bool isMoving;
    /// <summary>
    /// the last mouse pos
    /// </summary>
    sf::Vector2i lastPos;
};

