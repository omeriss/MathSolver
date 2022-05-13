#include "TextBox.h"

TextBox::TextBox(sf::Texture& texture, sf::Font& font, sf::Vector2f pos, sf::Vector2f size, string tempString, sf::Color textColor, float textSize, TextType textType) :UiElement(texture, pos, size)
{
	type = textType;

	text.setFont(font);
	text.setFillColor(textColor);
	text.setCharacterSize(textSize);
	//text.setPosition(pos.x+text.getLetterSpacing()*5, pos.y);
	text.setString("");

	tempTxt.setFont(font);
	tempTxt.setFillColor(sf::Color(200, 200, 200));
	tempTxt.setCharacterSize(textSize);
	//tempTxt.setPosition(pos.x + text.getLetterSpacing() * 5, pos.y);
	if (textType == NoEdit)
		text.setString(tempString);
	else
		tempTxt.setString(tempString);

	SetTextPosition(pos);

	writeMode = false;
	letMove = false;
}

void TextBox::AddToString(char c)
{
	if(onChar == -1)
		text.setString(c+text.getString());
	else
		text.setString(text.getString().substring(0, onChar+1) + c + text.getString().substring(onChar + 1, text.getString().getSize()-onChar-1));
}

std::string TextBox::GetText()
{
	return text.getString();
}

void TextBox::Clear()
{
	this->text.setString("");
}

void TextBox::SetString(std::string str)
{
	onChar = -1;
	this->text.setString(str);
}


void TextBox::Update(sf::RenderWindow& window)
{
	if (type == NoEdit)
		return;

	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		if (InsideSprite(mousePos)) {
			writeMode = true;

			int tLen = text.getString().getSize();
			onChar = -1;
			for (int i = 1; i <= tLen; i++) {
				if (mousePos.x < text.findCharacterPos(i).x && (mousePos.y < text.findCharacterPos(i).y + text.getCharacterSize() || !(type == Text))) {
					onChar = i - 1;
					if (text.findCharacterPos(i).x - mousePos.x > mousePos.x - text.findCharacterPos(i - 1).x)
						onChar--;
					break;
				}
				else if ((mousePos.y < text.findCharacterPos(i).y + text.getCharacterSize() || !(type == Text)) && (i == tLen || text.findCharacterPos(i).y != text.findCharacterPos(i + 1).y)) {
					onChar = i-1;
					break;
				}
			}
		}
		else {
			writeMode = false;
		}
	}
	if (writeMode) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && onChar != text.getString().getSize() - 1) {
			if (letMove) onChar++;
			letMove = false;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && onChar != 0) {
			if(letMove) onChar--;
			letMove = false;
		}
		else {
			letMove = true;
		}
		for (auto c : input) {
			switch (c)
			{
			case 13:
				if (type == Text) {
					text.setString(text.getString() + '\n');
					onChar++;
				}
				break;
			case 8:
				if (onChar != -1) {
					text.setString(text.getString().substring(0, onChar) + text.getString().substring(onChar + 1, text.getString().getSize() - onChar - 1));
					onChar--;
				}
				break;
			default:
				AddToString(c);
				onChar++;
				break;
			}

			sf::Vector2f endPosition = text.findCharacterPos(text.getString().getSize());
			endPosition.y += text.getCharacterSize();
			if (!InsideSprite(endPosition)) {
				text.setString(text.getString().substring(0, onChar) + text.getString().substring(onChar + 1, text.getString().getSize() - onChar - 1));
				onChar--;
			}

		}
	}
}

void TextBox::Print(sf::RenderWindow& window)
{
	window.draw(sprite);
	if (text.getString().getSize() == 0 && !writeMode)
		window.draw(tempTxt);
	else {
		/*
		if (type == Password) {
			sf::String passText = text.getString();
			for (auto& c : passText)
				c = '*';
			text.setString(passText);
			window.draw(text);
		}
		else
			window.draw(text);
		*/
		window.draw(text);
	}

	if (writeMode) {
		sf::RectangleShape PosRect;
		PosRect.setSize({ 3, (float)text.getCharacterSize() });
		PosRect.setOrigin(1.5, 0);
		PosRect.setFillColor(sf::Color(100, 100, 100));
		sf::Vector2f RectPos = text.findCharacterPos(onChar + 1);
		RectPos.x -= text.getLetterSpacing() / 2;
		PosRect.setPosition(RectPos);
		window.draw(PosRect);
	}
}

void TextBox::SetPosition(sf::Vector2f position)
{
	UiElement::SetPosition(position);
	SetTextPosition(position);
}

void TextBox::SetTextPosition(sf::Vector2f position)
{
	if (type == Text) {
		text.setPosition(position.x + text.getLetterSpacing() * 5, position.y);
		tempTxt.setPosition(position.x + text.getLetterSpacing() * 5, position.y);
	}
	else if (type == NoEdit) {
		text.setPosition(position.x, position.y);
	}
	else {
		text.setPosition(position.x + text.getLetterSpacing() * 30, position.y + sprite.getGlobalBounds().height / 2 - text.getCharacterSize() / 2 - text.getLineSpacing() * 10);
		tempTxt.setPosition(position.x + text.getLetterSpacing() * 30, position.y + sprite.getGlobalBounds().height / 2 - text.getCharacterSize() / 2 - text.getLineSpacing() * 10);
	}
}

void TextBox::SetOrigin(OriginType origin)
{
	sf::Vector2f oldOrigin = this->sprite.getOrigin();
	UiElement::SetOrigin(origin);
	text.setPosition(text.getPosition().x - (sprite.getOrigin().x - oldOrigin.x) * sprite.getScale().x, text.getPosition().y - (sprite.getOrigin().y - oldOrigin.y) * sprite.getScale().y);
	tempTxt.setPosition(tempTxt.getPosition().x - (sprite.getOrigin().x - oldOrigin.x) * sprite.getScale().x, tempTxt.getPosition().y - (sprite.getOrigin().y - oldOrigin.y) * sprite.getScale().y);
}