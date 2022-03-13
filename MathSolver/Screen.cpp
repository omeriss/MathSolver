#include "Screen.h"

Screen::Screen(int w, int h, sf::RenderWindow* window, ScreenType screenType)
{
	view.setSize(w, h);
	view.setCenter(w / 2, h / 2);
	this->winodw = window;
	isActive = true;
	this->screenType = screenType;
}

Screen::Screen(int w, int h, sf::FloatRect port, sf::RenderWindow* window, ScreenType screenType)
{
	view.setSize(w, h);
	view.setCenter(w / 2, h / 2);
	view.setViewport(port);
	this->winodw = window;
	isActive = true;
	this->screenType = screenType;
}

void Screen::ReScale(int width, int height, int lastWidth, int lastHeight)
{
	float scale = view.getSize().y * 1.0 / height;
	sf::FloatRect portCords;
	printf("%d %d\n", this->winodw->getSize().x, this->winodw->getSize().y);
	switch (screenType)
	{
	case AlignLeft:
		view.setSize(width * scale, view.getSize().y);
		view.setCenter(width * scale / 2, view.getCenter().y); // pin the screen to left
		break;
	case AlignRight:
		break;
	case AlignDown:
		break;
	case AlignUp:
		break;
	case AlignCenter:
		view.setSize(width * scale, view.getSize().y);
		break;
	case SubScreen:
		portCords = {view.getViewport().left * lastWidth, view.getViewport().top * lastHeight,
			view.getViewport().width*lastWidth, view.getViewport().height*lastHeight };
		view.setViewport({ portCords.left / width, portCords.top / height,
			portCords.width * (1.0f*height/lastHeight) / width, portCords.height * ((1.0f * height / lastHeight)) / height });
		break;
	default:
		break;
	}
}

void Screen::AddElement(UiElement* element)
{
	elements.push_back(element);
}

void Screen::UdpateScreen()
{
	if (!isActive)
		return;

	winodw->setView(view);
	for (auto e : elements) {
		e->Update(*winodw);
	}
}

void Screen::DrawScreen()
{
	if (!isActive)
		return;

	winodw->setView(view);
	for (auto e : elements) {
		e->Print(*winodw);
	}
}

void Screen::SetActive(bool isActive)
{
	this->isActive = isActive;
}

UiElement* Screen::GetElementByName(std::string name)
{
	for (auto& element : elements) {
		if (element->GetName() == name) {
			return element;
		}
	}
	return NULL;
}


