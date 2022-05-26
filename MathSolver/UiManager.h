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

	/// <summary>
	/// singelton instance
	/// </summary>
	/// <returns></returns>
	static UiManager* GetInstance();

	/// <summary>
	/// update events
	/// </summary>
	void UpdateSfmlEvents();
	void Init();
	void InitScreens();
	void Start();
	void Update();
	Meeting* GetMeeting();

	/// <summary>
	/// get the screens
	/// </summary>
	/// <returns> the screens</returns>
	std::map<std::string, Screen*>& GetScreens();
	Client cl;
private:

	/// <summary>
	/// constractor
	/// </summary>
	UiManager();
	static UiManager* instance;
	UiManager(UiManager& other) = delete;
	void operator=(const UiManager&) = delete;
	void DoLogIn();
	void DoSignUp();

	/// <summary>
	/// the meeting class
	/// </summary>
	Meeting* meeting;
	
	/// <summary>
	/// the screens
	/// </summary>
	std::map<std::string,Screen*> screens;
	sf::RenderWindow* window;
	sf::Vector2u lastWindowSize;
	asio::io_context pythonContext;
	pybind11::module_ fireBaseModule;
};