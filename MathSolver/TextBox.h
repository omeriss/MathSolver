#pragma once
#include <string>
#include "UiElement.h"
using namespace std;

typedef enum {Text, OneLine, Password, Number, NoEdit} TextType;

class TextBox :
    public UiElement
{
public:
    TextBox(sf::Texture& texture, sf::Font& font, sf::Vector2f pos = { 0,0 }, sf::Vector2f size = {200, 50}, string tempString = "", sf::Color textColor = sf::Color::Black, float textSize = 30, TextType textType = OneLine);
    void Update(sf::RenderWindow& window);
    void Print(sf::RenderWindow& window);
    void SetPosition(sf::Vector2f position);
    void SetTextPosition(sf::Vector2f position);
    void SetOrigin(OriginType origin);
    void AddToString(char c);
    std::string GetText();
    void Clear();
    void SetString(std::string str);
private:
    bool writeMode;
    sf::Text text;
    sf::Text tempTxt;
    TextType type;
    int onChar;
    bool letMove;
};

