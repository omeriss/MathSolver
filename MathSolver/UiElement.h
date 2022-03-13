#pragma once 
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <filesystem>
#include "constants.h"




typedef enum OriginType {
	TopLeft, TopRight, ButtomLeft, ButtomRight, Center
};

class UiElement
{
public:
	static std::vector<char> input;
	static std::map<std::string, sf::Texture*>& textureMap;
	UiElement(const sf::Texture& texture);
	UiElement(const sf::Texture& texture, sf::Vector2f pos, sf::Vector2f size);
	UiElement();
	virtual void Update(sf::RenderWindow& window)=0;
	virtual void Print(sf::RenderWindow& window)=0;
	virtual void SetOrigin(OriginType origin);
	bool InsideSprite(sf::Vector2f pos);
	void SetName(std::string name);
	std::string GetName();
protected:
	sf::Sprite sprite;
	std::string name;
	bool IsActive;
	
};