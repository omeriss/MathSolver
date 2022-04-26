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
	Screen(int w, int h, sf::RenderWindow* window, ScreenType screenType = AlignCenter);
	Screen(int w, int h, sf::FloatRect port, sf::RenderWindow* window, ScreenType screenType = AlignCenter);
	void ReScale(int width, int height, int lastWidth, int lastHeight);
	void AddElement(UiElement* element);
	void RemoveElement(UiElement* element);
	virtual void UdpateScreen();
	virtual void DrawScreen();
	void SetActive(bool isActive);
	UiElement* GetElementByName(std::string name);
protected:
	sf::View view;
	sf::RenderWindow* winodw;
	std::vector<UiElement*> elements;
	bool isActive;
	ScreenType screenType;
	friend class DragBar;
};

