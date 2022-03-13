#pragma once
#include "Client.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <map>
#include <filesystem>
#include <vector>
#include <string>
#include <algorithm>
#include <Windows.h>
#include "UiElement.h"
#include "TextBox.h"
#include "Screen.h"
#include "constants.h"
#include "Button.h"
#include "Image.h"
#include "Graph.h"
#include "DragBar.h"
#include "Client.h"

class UiManager
{
public:
	static UiManager* instance;
	UiManager();
	void UpdateSfmlEvents();
	void Init();
	void InitScreens();
	void Start();
	void Update();
	Client cl;
private:
	std::map<std::string,Screen*> screens;
	sf::RenderWindow* window;
	sf::Font baseFont;
	sf::Vector2u lastWindowSize;
};

