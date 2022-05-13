#include "UiElement.h"

UiElement::UiElement(const sf::Texture& texture)
{
	sprite.setTexture(texture);
	IsActive = true;
}

UiElement::UiElement(const sf::Texture& texture, sf::Vector2f pos, sf::Vector2f size)
{
	sprite.setTexture(texture);
	IsActive = true;
	sprite.setPosition(pos);
	sprite.setScale(size.x / texture.getSize().x, size.y / texture.getSize().y);
}

UiElement::UiElement()
{
	IsActive = true;
}

void UiElement::SetOrigin(OriginType origin)
{
	switch (origin)
	{
	case TopLeft:
		sprite.setOrigin(0, 0);
		break;
	case TopRight:
		sprite.setOrigin(sprite.getTexture()->getSize().x, 0);
		break;
	case ButtomLeft:
		sprite.setOrigin(0, sprite.getTexture()->getSize().y);
		break;
	case ButtomRight:
		sprite.setOrigin(sprite.getTexture()->getSize().x, sprite.getTexture()->getSize().y);
		break;
	case Center:
		sprite.setOrigin(sprite.getTexture()->getSize().x/2.0, sprite.getTexture()->getSize().y/2.0);
		break;
	default:
		break;
	}
}

void UiElement::SetPosition(sf::Vector2f position)
{
	sprite.setPosition(position);
}

sf::Vector2f UiElement::GetPosition()
{
	return sprite.getPosition();
}

bool UiElement::InsideSprite(sf::Vector2f pos)
{
	return enableMouseClick && MouseInScreen && pos.x >= sprite.getPosition().x - sprite.getOrigin().x * sprite.getScale().x && pos.y >= sprite.getPosition().y - sprite.getOrigin().y * sprite.getScale().y
		&& pos.x <= sprite.getPosition().x - sprite.getOrigin().x * sprite.getScale().x + sprite.getGlobalBounds().width
		&& pos.y <= sprite.getPosition().y - sprite.getOrigin().y * sprite.getScale().y + sprite.getGlobalBounds().height;
}

void UiElement::SetName(std::string name)
{
	this->name = name;
}

std::string UiElement::GetName()
{
	return name;
}

