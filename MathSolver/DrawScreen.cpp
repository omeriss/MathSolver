#include "DrawScreen.h"

DrawScreen::DrawScreen(sf::Vector2f pos, sf::Vector2f size, Screen* subScreen)
{
	canDraw = false;

	overlay.create(BASE_DRAW_SCREEN_W, BASE_DRAW_SCREEN_H);
	overlay.clear(sf::Color(0, 0, 0, 0));

	board.create(BASE_DRAW_SCREEN_W, BASE_DRAW_SCREEN_H * Draw_Screen_H_Mul);

	View.setSize(BASE_DRAW_SCREEN_W, BASE_DRAW_SCREEN_H * Draw_Screen_H_Mul);
	View.setCenter(BASE_DRAW_SCREEN_W/2, BASE_DRAW_SCREEN_H * Draw_Screen_H_Mul /2);
	board.setView(View);

	sprite.setPosition(pos);
	sprite.setTexture(board.getTexture());

	OverlaySprite.setPosition(pos);

	lastPrassed = false;

	pixel = sf::RectangleShape({1,1 });
	pixel.setFillColor(sf::Color::Black);

	drawRadius = 1;

	drawingRect = false;
	boardPixels.resize(BASE_DRAW_SCREEN_H * Draw_Screen_H_Mul);
	Clear();
	this->clinet == nullptr;
}

void DrawScreen::Update(sf::RenderWindow& window)
{
	auto mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && this->InsideSprite(mousePos)) {
		if (!drawingRect) {
			drawingRect = true;
			rectStart = mousePos;
		}
		else {
			overlay.clear(sf::Color(0, 0, 0, 0));
			OverlaySprite.setPosition({ 0,0 });
			sf::RectangleShape markedRect = sf::RectangleShape({ std::abs(mousePos.x - rectStart.x), std::abs(mousePos.y - rectStart.y) });
			markedRect.setPosition({ std::min(mousePos.x, rectStart.x), std::min(mousePos.y, rectStart.y) });
			markedRect.setFillColor(MarkedColor);
			overlay.draw(markedRect);
			overlay.display();
			OverlaySprite.setTexture(overlay.getTexture());
		}
	}
	else if (!sf::Mouse::isButtonPressed(sf::Mouse::Right) && drawingRect && this->InsideSprite(mousePos)) {
		drawingRect = false;

		overlay.clear(sf::Color(0, 0, 0, 0));
		auto results = FindChars(sf::IntRect(std::min(mousePos.x, rectStart.x), std::min(mousePos.y, rectStart.y) - sprite.getPosition().y,
			std::ceil(std::abs(mousePos.x - rectStart.x)), std::ceil(std::abs(mousePos.y - rectStart.y))));

		this->equationAnalizer.LoadFromImages(results);

		for (auto const& rect : results) {
			sf::RectangleShape tempRect(sf::Vector2f(rect.first.width, rect.first.height));
			tempRect.setPosition(rect.first.left, rect.first.top + sprite.getPosition().y);
			tempRect.setFillColor(sf::Color(0, 0, 0, 0));
			tempRect.setOutlineColor(sf::Color::Red);
			tempRect.setOutlineThickness(3);
			overlay.draw(tempRect);
		}

		overlay.display();
		OverlaySprite.setTexture(overlay.getTexture());
	}

	if (UiElement::MouseWheelDelta && InsideSprite(mousePos)) {
		float spriteY = sprite.getPosition().y + UiElement::MouseWheelDelta * MouseWheelSensitivity;
		spriteY = (spriteY < 0) ? spriteY : 0;
		spriteY = (spriteY > -1 * BASE_DRAW_SCREEN_H * Draw_Screen_H_Mul) ? spriteY : -1 * BASE_DRAW_SCREEN_H * Draw_Screen_H_Mul;
		sprite.setPosition(sprite.getPosition().x, spriteY);
		sprite.setTexture(board.getTexture());
	}

	if(canDraw)
		Draw(window);
}

void DrawScreen::DrawAround(int x, int y)
{
	if (drawRadius == 0) {
		DrawAndSend(x, y);
		return;
	}
	int fromX = x - drawRadius;
	int fromY = y;
	while (fromX < x) {
		while (pow(fromX - x, 2) + pow(fromY - y, 2) > pow(drawRadius, 2) && fromX < x) {
			fromX++;
		}
		int tempX = fromX;
		while (tempX <= x) {
			DrawAndSend(tempX, fromY);
			DrawAndSend(x + (x-tempX), fromY);
			DrawAndSend(x + (x - tempX), y + (y - fromY));
			DrawAndSend(tempX, y + (y - fromY));
			tempX++;
		}
		fromY--;
	}
}

void DrawScreen::DrawAndSend(int x, int y)
{
	if (y < BASE_DRAW_SCREEN_H * Draw_Screen_H_Mul && x < BASE_DRAW_SCREEN_W && x >= 0 && y >= 0) {
		if (boardPixels[y][x].toInteger() == pixel.getFillColor().toInteger())
			return;
		pixel.setPosition(x, y);
		boardPixels[y][x] = pixel.getFillColor();
		board.draw(pixel);
		if (clinet) {
			std::shared_ptr<Packet> packet = std::make_shared<Packet>();
			packet->GetHeader().packetType = SendCell;
			(*packet) << x << y << pixel.getFillColor();
			clinet->SendTcp(std::move(packet));
		}
	}
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
		DrawAround(lastPos.x, lastPos.y);
		//if (lastPos.y < BASE_DRAW_SCREEN_H * Draw_Screen_H_Mul && lastPos.x < BASE_DRAW_SCREEN_W) {
		//	pixel.setPosition(floor(lastPos.x), floor(lastPos.y));
		//	boardPixels[lastPos.y][lastPos.x] = pixel.getFillColor();
		//	board.draw(pixel);
		//	if (clinet) {
		//		std::shared_ptr<Packet> packet = std::make_shared<Packet>();
		//		packet->GetHeader().packetType = SendCell;
		//		(*packet) << int(lastPos.x) << int(lastPos.y) << pixel.getFillColor();
		//		clinet->SendTcp(std::move(packet));
		//	}
		//}
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
	board.display();
	sprite.setTexture(board.getTexture());
}

void DrawScreen::SetLine(int y, array<sf::Color, BASE_DRAW_SCREEN_W>& pixels)
{
	boardPixels[y] = pixels;
	sf::Color temp = pixel.getFillColor();

	int x = 0;
	for (auto c : pixels) {
		if (c != sf::Color::White) {
			this->boardPixels[y][x] = c;
			pixel.setPosition(x, y);
			pixel.setFillColor(c);
			board.draw(pixel);
		}
		x++;
	}

	pixel.setFillColor(temp);
	board.display();
	sprite.setTexture(board.getTexture());
}

void DrawScreen::SetClinet(Client* cl)
{
	this->clinet = cl;
}

void DrawScreen::SetColor(sf::Color color)
{
	pixel.setFillColor(color);
}

void DrawScreen::SetRadius(float r)
{
	this->drawRadius = r;
}

void DrawScreen::SetDrawMode(bool canDraw)
{
	this->canDraw = canDraw;
}

void DrawScreen::Clear()
{
	board.clear(BaseDrawScreenColor);
	for (auto& row : boardPixels) {
		for (auto& pix : row) {
			pix = BaseDrawScreenColor;
		}
	}
}

void DrawScreen::SetSubScreen(Screen* subScreen)
{
	equationAnalizer.SetSubScreen(subScreen);
}

vector<array<sf::Color, BASE_DRAW_SCREEN_W>>& DrawScreen::GetBoard()
{
	return boardPixels;
}

#define DirLen 8
static int xDir[DirLen] = { 0, 0, 1, 1, 1, -1, -1, -1 };
static int yDir[DirLen] = { -1, 1, -1, 0, 1, -1, 0, 1 };

std::pair<sf::IntRect, double*> DrawScreen::findChar(int startX, int startY, sf::IntRect area, bool* done)
{
	int minX = startX, minY = startY, maxX = startX, maxY = startY;
	std::stack<sf::Vector2i> nextFilledPositions;
	std::stack<sf::Vector2i> positions;
	nextFilledPositions.push({ startX, startY});

	while (!nextFilledPositions.empty()) {
		int x = nextFilledPositions.top().x;
		int y = nextFilledPositions.top().y;
		nextFilledPositions.pop();
		if (x < area.left || y < area.top || x >= area.left + area.width || y >= area.top + area.height)
			continue;
		if (this->boardPixels[y][x].toInteger() == BaseDrawScreenColor.toInteger() || done[(y - area.top) * area.width + (x - area.left)])
			continue;
		positions.push({ x, y });
		done[(y - area.top) * area.width + (x - area.left)] = true;
		minX = std::min(minX, x);
		maxX = std::max(maxX, x);
		minY = std::min(minY, y);
		maxY = std::max(maxY, y);
		for (int i = 0; i < DirLen; i++) {
			nextFilledPositions.push({ x + xDir[i], y + yDir[i] });
		}
	}

	double* image = new double[CharImageH * CharImageW];
	std::memset(image, 0, CharImageH * CharImageW * sizeof(double));

	float OgImgW = (maxX - minX + 1), OgImgH = (maxY - minY + 1);
	float ImageScaleX = 1.0 * CharImageW / OgImgW, ImageScaleY = 1.0 * CharImageH / OgImgH, ImgScale;
	int XInc = 0, YInc = 0;

	if (ImageScaleX < ImageScaleY) {
		ImgScale = ImageScaleX;
		YInc = (OgImgW - OgImgH) * ImgScale / 2;
	}
	else {
		ImgScale = ImageScaleY;
		XInc = (OgImgH - OgImgW) * ImgScale / 2;
	}
	if (ImgScale > 1) {
		std::cout << "The Char Is too Small" << std::endl;
		bool* tempImage = new bool[long long(OgImgW) * long long(OgImgH)];
		std::fill(tempImage, tempImage + long long(OgImgW) * long long(OgImgH) * sizeof(bool), false);
		while (!positions.empty()) {
			int x = positions.top().x - minX;
			int y = positions.top().y - minY;
			positions.pop();
			tempImage[x * ((int)(OgImgH)) + y] = true;
		}
		for (int y = YInc; y < CharImageH - YInc; y++) {
			for (int x = XInc; x < CharImageW - XInc; x++) {
				if((int((x - XInc) / ImgScale)) * (int(OgImgH)) + int((y - YInc) / ImgScale) < OgImgW* OgImgH)
					image[(x) *CharImageH+y] = tempImage[(int((x-XInc)/ ImgScale))*(int(OgImgH)) + int((y-YInc) / ImgScale)];
			}
		}
		delete[] tempImage;
	}
	else {
		while (!positions.empty()) {
			int x = positions.top().x - minX;
			int y = positions.top().y - minY;
			positions.pop();
			x = x * ImgScale + XInc;
			y = y * ImgScale + YInc;
			image[x * (CharImageH)+y] = 1;
		}
	}

	
	if (FolderToSaveImgs != "") {
		sf::Image tempImg;
		tempImg.create(CharImageH, CharImageW);
		for (int x = 0; x < CharImageW; x++) {
			for (int y = 0; y < CharImageH; y++) {
				tempImg.setPixel(x, y, sf::Color(image[x * (CharImageH)+y]*255, image[x * (CharImageH)+y]*255, image[x * (CharImageH)+y]*255));
			}
		}
		std::string imgPath = FolderToSaveImgs;
		imgPath += "\\img" + std::to_string(rand()) + ".png";
		tempImg.saveToFile(imgPath);
	}

	return { {minX, minY, maxX - minX + 1, maxY - minY + 1}, image };
}

vector<std::pair<sf::IntRect, double*>> DrawScreen::FindChars(sf::IntRect area)
{
	vector<std::pair<sf::IntRect, double*>> results;
	bool* done = new bool[area.width*area.height];
	std::memset(done, false, area.width*area.height * sizeof(bool));
	for (int y = area.top; y < area.top + area.height; y++) {
		for (int x = area.left; x < area.left + area.width; x++) {
			if (!done[(y - area.top) * area.width + (x - area.left)] && this->boardPixels[y][x].toInteger() != BaseDrawScreenColor.toInteger()) {
				results.push_back(findChar(x, y, area, done));
			}
		}
	}
	delete[] done;
	return results;
}

void DrawScreen::Draw(sf::RenderWindow& window) {
	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

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
	window.draw(OverlaySprite);
}