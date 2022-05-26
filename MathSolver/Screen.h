#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "UiElement.h"

typedef enum ScreenType {
	AlignLeft = 0, AlignRight = 1, AlignDown = 2, AlignUp = 3, AlignCenter = 4, SubScreen = 5, ScaleByWith = 6
};

class Screen
{
public:
	/// <summary>
	/// constructor
	/// </summary>
	/// <param name="w"> w </param>
	/// <param name="h"> h </param>
	/// <param name="window"> the main wind</param>
	/// <param name="screenType"> type</param>
	Screen(int w, int h, sf::RenderWindow* window, ScreenType screenType = AlignCenter);
	Screen(int w, int h, sf::FloatRect port, sf::RenderWindow* window, ScreenType screenType = AlignCenter);

	/// <summary>
	/// change scale
	/// </summary>
	/// <param name="width"></param>
	/// <param name="height"></param>
	/// <param name="lastWidth"></param>
	/// <param name="lastHeight"></param>
	void ReScale(int width, int height, int lastWidth, int lastHeight);
	void AddElement(UiElement* element);
	void RemoveElement(UiElement* element);
	void clear();

	/// <summary>
	/// update the ui elements
	/// </summary>
	virtual void UdpateScreen();

	/// <summary>
	/// draw ui
	/// </summary>
	virtual void DrawScreen();
	void SetActive(bool isActive);
	bool IsActive();
	void SwapUiElements(std::vector<UiElement*>& elements);
	sf::FloatRect GetPort();
	void SetPort(sf::FloatRect port);
	UiElement* GetElementByName(std::string name);
protected:

	/// <summary>
	/// the view of the screen
	/// </summary>
	sf::View view;
	sf::RenderWindow* winodw;
	std::vector<UiElement*> elements;
	bool isActive;
	ScreenType screenType;
	friend class DragBar;
};

