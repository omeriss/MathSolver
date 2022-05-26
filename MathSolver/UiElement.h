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

	/// <summary>
	/// static input of all elements
	/// </summary>
	static std::vector<char> input;
	static bool MouseInScreen;
	static int MouseWheelDelta;

	/// <summary>
	/// all the textures
	/// </summary>
	static std::map<std::string, sf::Texture*>& textureMap;
	static sf::Font baseFont;
	static bool enableMouseClick;

	/// <summary>
	/// constructor
	/// </summary>
	/// <param name="texture"> texture</param>
	UiElement(const sf::Texture& texture);
	UiElement(const sf::Texture& texture, sf::Vector2f pos, sf::Vector2f size);
	UiElement();

	/// <summary>
	/// functions to implement on elemets
	/// </summary>
	 
	virtual void Update(sf::RenderWindow& window)=0;
	virtual void Print(sf::RenderWindow& window)=0;
	virtual void SetOrigin(OriginType origin);
	virtual void SetPosition(sf::Vector2f position);
	virtual sf::Vector2f GetPosition();

	/// <summary>
	/// is pos insdide the sprite
	/// </summary>
	/// <param name="pos"> the pos</param>
	/// <returns></returns>
	bool InsideSprite(sf::Vector2f pos);
	void SetName(std::string name);
	void SetActive(bool isActive);
	std::string GetName();
protected:

	/// <summary>
	/// the main sprite
	/// </summary>
	sf::Sprite sprite;
	std::string name;
	bool IsActive;

	
};