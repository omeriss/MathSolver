#include "DrawScreen.h"

DrawScreen::DrawScreen(sf::Vector2f pos, sf::Vector2f size)
{
	board.create(BASE_DRAW_SCREEN_W, BASE_DRAW_SCREEN_H * Draw_Screen_H_Mul);
	View.setSize(BASE_DRAW_SCREEN_W, BASE_DRAW_SCREEN_H * Draw_Screen_H_Mul);
	View.setCenter(BASE_DRAW_SCREEN_W/2, BASE_DRAW_SCREEN_H * Draw_Screen_H_Mul /2);
	board.setView(View);
	sprite.setPosition(pos);
	sprite.setTexture(board.getTexture());
	board.clear(sf::Color(255, 255, 255));
	lastPrassed = false;
	pixel = sf::RectangleShape({1,1 });
	boardPixels.resize(BASE_DRAW_SCREEN_H * Draw_Screen_H_Mul);
	this->clinet == nullptr;
	for (auto& row : boardPixels) {
		for (auto& pix : row) {
			pix = sf::Color::White;
		}
	}
}

void DrawScreen::Update(sf::RenderWindow& window)
{
	Draw(window);
}


void DrawScreen::DrawLine(sf::Vector2f pos)
{
	sf::Vector2f dir = pos - lastPos;
	float len = 0;
	float rayLenSqure = dir.x * dir.x + dir.y * dir.y;
	dir /= (sqrt(rayLenSqure));
	sf::Vector2f UnitLen = {sqrt(pow((dir.y/dir.x), 2.0f) + 1), sqrt(pow((dir.x / dir.y), 2.0f) + 1)};
	sf::Vector2f ray = { ((dir.x < 0) ? lastPos.x - floor(lastPos.x) : floor(lastPos.x + 1) - lastPos.x) * UnitLen.x,
	((dir.y < 0) ? lastPos.y - floor(lastPos.y) : floor(lastPos.y + 1) - lastPos.y)* UnitLen.y };
	while (len*len <= rayLenSqure)
	{
		if (lastPos.y < BASE_DRAW_SCREEN_H * Draw_Screen_H_Mul && lastPos.x < BASE_DRAW_SCREEN_W) {
			pixel.setPosition(floor(lastPos.x), floor(lastPos.y));
			boardPixels[lastPos.y][lastPos.x] = pixel.getFillColor();
			board.draw(pixel);
			if (clinet) {
				std::shared_ptr<Packet> packet = std::make_shared<Packet>();
				packet->GetHeader().packetType = SendCell;
				(*packet) << int(lastPos.x) << int(lastPos.y) << pixel.getFillColor();
				clinet->SendTcp(std::move(packet));
			}
		}
		if (ray.x < ray.y) {
			lastPos.x += (dir.x > 0)*2-1;
			len = ray.x;
			ray.x += UnitLen.x;
		}
		else {
			lastPos.y += (dir.y > 0) * 2 - 1;
			len = ray.y;
			ray.y += UnitLen.y;
		}
	}
}

void DrawScreen::SetCell(int x, int y, sf::Color color)
{
	boardPixels[y][x] = color;
	sf::Color temp = pixel.getFillColor();
	pixel.setFillColor(color);
	pixel.setPosition(x, y);
	board.draw(pixel);
	pixel.setFillColor(temp);
}

void DrawScreen::SetLine(int y, array<sf::Color, BASE_DRAW_SCREEN_W>& pixels)
{
	boardPixels[y] = pixels;
	sf::Color temp = pixel.getFillColor();
	
	int x = 0;
	for (auto c : pixels) {
		if (c != sf::Color::White) {
			pixel.setPosition(x, y);
			pixel.setFillColor(c);
			board.draw(pixel);
		}
		x++;
	}

	pixel.setFillColor(temp);
}

void DrawScreen::SetClinet(Client* cl)
{
	this->clinet = cl;
}

vector<array<sf::Color, BASE_DRAW_SCREEN_W>>& DrawScreen::GetBoard()
{
	return boardPixels;
}

void DrawScreen::Draw(sf::RenderWindow& window) {
	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	if (UiElement::MouseWheelDelta && InsideSprite(mousePos)) {
		float spriteY = sprite.getPosition().y + UiElement::MouseWheelDelta * MouseWheelSensitivity;
		spriteY = (spriteY < 0) ? spriteY  : 0;
		spriteY = (spriteY > -1 * BASE_DRAW_SCREEN_H * Draw_Screen_H_Mul) ? spriteY : -1 * BASE_DRAW_SCREEN_H * Draw_Screen_H_Mul;
		sprite.setPosition(sprite.getPosition().x, spriteY);
		sprite.setTexture(board.getTexture());
	}
	pixel.setFillColor(sf::Color::Black);
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)
		&& InsideSprite(mousePos)) {
		mousePos.y -= sprite.getPosition().y;
		if (lastPrassed) {
			DrawLine(mousePos);
		}
		lastPos = mousePos;
		lastPrassed = true;
		board.display();
		sprite.setTexture(board.getTexture());
	}
	else {
		lastPrassed = false;
	}
}


void DrawScreen::Print(sf::RenderWindow& window)
{
	window.draw(sprite);
}