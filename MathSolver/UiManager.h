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
#include <pybind11/pybind11.h>
#include <pybind11/embed.h>
#include <Python.h>
#include "UiElement.h"
#include "TextBox.h"
#include "Screen.h"
#include "constants.h"
#include "Button.h"
#include "Image.h"
#include "Graph.h"
#include "DragBar.h"
#include "Client.h"
#include "DrawScreen.h"
#include "Meeting.h"
#include "ScrollBar.h"

class UiManager
{
public:
	static UiManager* GetInstance();
	void UpdateSfmlEvents();
	void Init();
	void InitScreens();
	void Start();
	void Update();
	Meeting* GetMeeting();
	std::map<std::string, Screen*>& GetScreens();
	Client cl;
private:
	UiManager();
	static UiManager* instance;
	UiManager(UiManager& other) = delete;
	void operator=(const UiManager&) = delete;
	void DoLogIn();
	void DoSignUp();
	Meeting* meeting;
	std::map<std::string,Screen*> screens;
	sf::RenderWindow* window;
	sf::Vector2u lastWindowSize;
	asio::io_context pythonContext;
	pybind11::module_ fireBaseModule;
	std::string userName;
};