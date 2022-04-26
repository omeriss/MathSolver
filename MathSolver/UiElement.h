#pragma once 
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <atomic>
#include <filesystem>
#include "constants.h"




typedef enum OriginType {
	TopLeft, TopRight, ButtomLeft, ButtomRight, Center
};

class UiElement
{
public:
	static std::vector<char> input;
	static bool MouseInScreen;
	static int MouseWheelDelta;
	static std::map<std::string, sf::Texture*>& textureMap;
	UiElement(const sf::Texture& texture);
	UiElement(const sf::Texture& texture, sf::Vector2f pos, sf::Vector2f size);
	UiElement();
	virtual void Update(sf::RenderWindow& window)=0;
	virtual void Print(sf::RenderWindow& window)=0;
	virtual void SetOrigin(OriginType origin);
	virtual void SetPosition(sf::Vector2f position);
	virtual sf::Vector2f GetPosition();
	bool InsideSprite(sf::Vector2f pos);
	void SetName(std::string name);
	std::string GetName();
protected:
	sf::Sprite sprite;
	std::string name;
	bool IsActive;

	
};