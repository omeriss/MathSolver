#include "Graph.h"

#define PixDis 0.0007
const pair<float, float> Graph::changes[12] = { 
	{-PixDis, -PixDis}, {PixDis, PixDis},{0, -2*PixDis}, {0, -2*PixDis}, {-2*PixDis, 0}, {2*PixDis, 0}, {PixDis, -PixDis} , {-PixDis, PixDis}, {-PixDis, 0}, {PixDis, 0}, {0, PixDis}, {0, -PixDis} };

Graph::Graph(sf::Vector2f pos, sf::Vector2f size, std::vector<CalcElement>& Fx)
{
	this->renderTex.create(size.x, size.y );
	GraphView.setSize(size.x, size.y);
	GraphView.setCenter(0, 0);
	renderTex.setView(GraphView);
	this->Fx = Fx;
	sprite.setPosition(pos);
	moving = false;
	UpdateNextFrame = true;
	xWeight = StartGraphXW;
	sprite.setTexture(renderTex.getTexture());
}

void Graph::Update(sf::RenderWindow& window)
{

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
		xWeight += 2;
		UpdateNextFrame = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
		xWeight -= 2;
		UpdateNextFrame = true;
	}

	MoveGraph(window);

	if (!UpdateNextFrame)
		return;

	bool LastPointExists = true;
	int GoingUp = -1;
	float LastY;
	pair<float, int> ans;
	sf::Vertex line[2];

	renderTex.clear(sf::Color::Black);

	int i = -GraphDensity/2;
	while (i < GraphDensity && !(ans = Calc(GraphView.getCenter().x / (1.0f * renderTex.getSize().x / xWeight) + ((float)i++)*(1.0f* xWeight / (1.0f * GraphDensity + !(GraphDensity % 2))))).second) {}
	LastY = ans.first;
	float temp = GraphView.getCenter().x / (1.0f * renderTex.getSize().x / xWeight) + ((float)i) * (1.0f * xWeight / (1.0f * GraphDensity + !(GraphDensity % 2)));
	line[0] = sf::Vector2f(temp * (1.0f * renderTex.getSize().x / xWeight), (-ans.first * (1.0f * renderTex.getSize().x / xWeight)));
	line[0].color = sf::Color::Red;

	for (; i <= GraphDensity/2; i++) {
		float x = GraphView.getCenter().x / (1.0f * renderTex.getSize().x / xWeight) + ((float)i) * (1.0f * xWeight / (1.0f * GraphDensity + !(GraphDensity % 2)));
		ans = Calc(x);
		line[1] = sf::Vector2f(x*(1.0f* renderTex.getSize().x / xWeight), (-ans.first *(1.0f* renderTex.getSize().x / xWeight)));
		line[1].color = sf::Color::Red;
		if (LastPointExists && ans.second) {
			if (GoingUp != (LastY < ans.first) && (LastY < 0) != (ans.first < 0) && abs(LastY - ans.first) > 1.0*AsymptoteThreshold/xWeight) {
				GoingUp = LastY < ans.first;
				LastY = ans.first;
				LastPointExists = false;
				continue;
			}
			renderTex.draw(line, 2, sf::Lines);

			//
			for (int j = 0; j < 8; j++) {
				sf::Vertex tempLine[2] = {
					{ {line[0].position.x + renderTex.getSize().x * this->changes[j].first, line[0].position.y + renderTex.getSize().y * this->changes[j].second}, sf::Color::Red },
					{ {line[1].position.x + renderTex.getSize().x * this->changes[j].first, line[1].position.y + renderTex.getSize().y * this->changes[j].second}, sf::Color::Red } };
				renderTex.draw(tempLine, 2, sf::Lines);
			}
			//

			GoingUp = LastY < ans.first;
			LastY = ans.first;
		}
		if (ans.second) {
			line[0] = line[1];
			LastPointExists = true;
		}
		else
			LastPointExists = false;
	}

	// draw the ax
	sf::Vertex lineX[2] = { {{-10e+10, 0}, sf::Color::White}, {{10e+10, 0}, sf::Color::White} };
	renderTex.draw(lineX, 2, sf::Lines);
	sf::Vertex lineY[2] = { {{0,-10e+10 }, sf::Color::White}, {{0,10e+10}, sf::Color::White} };
	renderTex.draw(lineY, 2, sf::Lines);
	for (int j = 0; j < 6; j++) {
		sf::Vertex tempLineX[2] = {
			{ {lineX[0].position.x + renderTex.getSize().x * this->changes[j].first, lineX[0].position.y + renderTex.getSize().y * this->changes[j].second}, sf::Color::White },
			{ {lineX[1].position.x + renderTex.getSize().x * this->changes[j].first, lineX[1].position.y + renderTex.getSize().y * this->changes[j].second}, sf::Color::White } };
		renderTex.draw(tempLineX, 2, sf::Lines);
		sf::Vertex tempLineY[2] = {
				{ {lineY[0].position.x + renderTex.getSize().x * this->changes[j].first, lineY[0].position.y + renderTex.getSize().y * this->changes[j].second}, sf::Color::White },
				{ {lineY[1].position.x + renderTex.getSize().x * this->changes[j].first, lineY[1].position.y + renderTex.getSize().y * this->changes[j].second}, sf::Color::White } };
		renderTex.draw(tempLineY, 2, sf::Lines);
	}
	renderTex.display();
	UpdateNextFrame = false;
}

void Graph::Print(sf::RenderWindow& window)
{
	sprite.setTexture(renderTex.getTexture());
	window.draw(sprite);
}

void Graph::MoveGraph(sf::RenderWindow& window)
{
	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)
		&& InsideSprite(mousePos)) {
		if (moving) {
			GraphView.move(lastPos.x - mousePos.x, lastPos.y - mousePos.y);
			renderTex.setView(GraphView);
		}
		else
			moving = true;
		lastPos = mousePos;
	}
	else
		moving = false;

	UpdateNextFrame |= moving;
}

pair<float, bool> getVal(float a, float b, char op) {
	switch (op)
	{
	case '+': return { a + b, true };
	case '-':return { a - b, true };
	case '*':return { a * b, true };
	case '/':return { a / b, true };
	case '^':return { pow(a, b), true };
	}
}

pair<float, bool> Graph::Calc(float x)
{
	stack<float> s;
	for (auto e : Fx)
	{
		if (e.IsNum) {
			s.push(e.Element.num);
		}
		else {
			if (e.Element.op == 'x')
				s.push(x);
			else {
				float n1 = s.top();
				s.pop();
				float n2 = s.top();
				s.pop();
				pair<float, bool> temp = getVal(n2, n1, e.Element.op);
				if (!temp.second)
					return temp;
				s.push(temp.first);
			}
		}
	}
	return { s.top(), true };
}
