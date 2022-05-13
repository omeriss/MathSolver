#include "EquationAnalizer.h"

std::vector<CalcElement> getPostFix(string s) {
	const int NUM = 1, X = 2, OPER = 3;
	std::stack<CalcElement> opStack;
	std::vector<CalcElement> result;
	int stringLen = s.size();
	int last = 0;
	int mulBy = 1;
	for (int i = 0; i < stringLen; i++) {
		//std::cout << i << std::endl;
		if (s[i] == 'y') {
			throw std::invalid_argument("Can't have y here");
		}
		if (s[i] <= '9' && s[i] >= '0') {
			if (last == X) {
				throw std::invalid_argument("Equation is wrong");
			}

			CalcElement ele;
			ele.Element.num = 0;
			ele.IsNum = true;
			while (i < stringLen && s[i] <= '9' && s[i] >= '0') {
				ele.Element.num *= 10;
				ele.Element.num += s[i] - '0';
				i++;
			}
			ele.Element.num *= mulBy;
			mulBy = 1;
			i--;
			result.push_back(ele);
			last = NUM;
		}
		else if (s[i] == 'x' && last != NUM) {
			if (last == X) {
				throw std::invalid_argument("Equation is wrong");
			}

			CalcElement ele;
			ele.IsNum = false;
			ele.Element.op = 'x';
			result.push_back(ele);
			last = X;
		}
		else {
			CalcElement ele;
			ele.IsNum = false;
			if (s[i] == '(') {
				if (last == X || last == NUM) {
					ele.Element.op = '*';
					i--;
				}
				else {
					ele.Element.op = '(';
					opStack.push(ele);
					last = 0;
					continue;
				}
			}
			else if (s[i] == ')') {
				while (!opStack.empty() && opStack.top().Element.op != '(') {
					result.push_back(opStack.top());
					opStack.pop();
				}
				if (!opStack.empty() && opStack.top().Element.op == '(') {
					opStack.pop();
				}
				else {
					throw std::invalid_argument("Equation is wrong");
				}
				last = NUM;
				continue;
			}
			else if (s[i] == 'x') {
				ele.Element.op = '*';
				i--;
			}
			else {
				ele.Element.op = s[i];
			}
			if (last == 0 || last == OPER) {
				if (ele.Element.op == '-' && i < stringLen - 1 && ((s[i + 1] <= '9' && s[i + 1] >= '0') || s[i + 1] == 'x'))
					mulBy = -1;
				else
					throw std::invalid_argument("Equation is wrong");
			}
			while (!opStack.empty() && opStack.top().Element.op != '(' && opRank[opStack.top().Element.op] >= opRank[ele.Element.op]) {
				result.push_back(opStack.top());
				opStack.pop();
			}
			opStack.push(ele);
			last = OPER;
		}

	}
	while (!opStack.empty()) {
		result.push_back(opStack.top());
		opStack.pop();
	}
	return result;
}


EquationAnalizer::EquationAnalizer()
{
	network = new NeuralNetwork(NNUploadFile);
	this->subScreen = nullptr;
	onEdit = true;
}

#define CenterX(rect) ((rect.left+(rect.left+rect.width))/2.0)
#define CenterY(rect) ((rect.top+(rect.top+rect.hight))/2.0)

bool RectCmp(std::pair<sf::IntRect, double*> r1, std::pair<sf::IntRect, double*> r2) {
	return r1.first.left < r2.first.left;
}

bool ResultCmp(pair<char, double> p1, pair<char, double> p2) {
	return p1.second > p2.second;
}

sf::IntRect GetCombinedRect(sf::IntRect a, sf::IntRect b) {
	int x = std::min(a.left, b.left);
	int y = std::min(a.top, b.top);
	int w = std::max(a.left + a.width, b.left + b.width) - x;
	int h = std::max(a.top + a.height, b.top + b.height) - y;
	return { x, y, w, h };
}

std::pair<int, pair<double, char>> GetCharFromResults(vector<pair<char, double>> nnResults, char find) {
	int resLen = nnResults.size();
	for (int i = 0; i < resLen; i++) {
		if (nnResults[i].first == find) {
			return { 0, nnResults[i] };
		}
	}
	return { -1, {-1, -1} };
}

void EquationAnalizer::LoadFromImages(std::vector<std::pair<sf::IntRect, double*>> images)
{
	try {
		if(images.size() <= 2){
			throw "not long enough";
		}
		leftSide = "", rightSide = "";
		std::sort(images.begin(), images.end(), RectCmp);
		int imageCount = images.size();

		vector<vector<pair<char, double>>> nnResults;

		for (int i = 0; i < imageCount; i++) {
			vector<pair<char, double>> nnResult;
			auto temp = network->Calc(images[i].second);
			for (int j = 0; j < temp.size(); j++) {
				nnResult.push_back({symbols[j], temp[j]});
			}
			std::sort(nnResult.begin(), nnResult.end(), ResultCmp);
			nnResults.push_back(nnResult);
		}

		double bestScore = 50;
		int bestIndex = -1;
		auto lastFindRes = GetCharFromResults(nnResults[0], '-');
		for(int i = 1; i < imageCount; i++) {
			auto findRes = GetCharFromResults(nnResults[i], '-');
			if (findRes.first < 2 && lastFindRes.first < 2) {
				if (CenterX(images[i].first) >= images[i - 1].first.left && CenterX(images[i].first) <= images[i - 1].first.left + images[i - 1].first.width &&
					CenterX(images[i - 1].first) >= images[i].first.left && CenterX(images[i - 1].first) <= images[i].first.left + images[i].first.width) {
					double score = (findRes.first != 0) + (lastFindRes.first != 0);
					score += std::abs(CenterX(images[i].first) - CenterX(images[i - 1].first)) / std::min(images[i].first.width, images[i-1].first.width);
					score += std::abs(images[i].first.width - images[i - 1].first.width) / std::min(images[i].first.width, images[i - 1].first.width);
					if (score < bestScore) {
						bestScore = score;
						bestIndex = i;
					}
				}
			}

			lastFindRes = findRes;
		}
		if(bestIndex == -1)
			throw std::invalid_argument("No = sign");

		for (int i = 0; i < bestIndex - 1; i++) {
			leftSide += nnResults[i][0].first;
		}
		for (int i = bestIndex + 1; i < imageCount; i++) {
			rightSide += nnResults[i][0].first;
		}
	}
	catch(...) {
		std::cout << "Can't solve this" << std::endl;
	}

	std::cout << leftSide << "=" << rightSide << std::endl;
	subScreen->SetActive(true);
	SwapScreens(true);
	TextBox* eqTextBox = ((TextBox*)this->subScreen->GetElementByName("eqTextBox"));
	std::string completeString = leftSide + "=" + rightSide;
	if (eqTextBox) {
		eqTextBox->SetString(completeString);
	}
}

void EquationAnalizer::LoadFromString(std::string equation)
{
	std::string errorString = "";
	int splitIndex = -1;
	for (int i = 0; i < equation.size(); i++) {
		if (std::find(okCharsFromEqString.begin(), okCharsFromEqString.end(), equation[i]) == okCharsFromEqString.end()) {
			errorString = "Equation is wrong";
			break;
		}
		if (equation[i] == '=') {
			if (splitIndex == -1)
				splitIndex = i;
			else {
				errorString = "can't have more than one =";
				break;
			}
		}
	}

	leftSide = equation.substr(0, splitIndex);
	rightSide = equation.substr(splitIndex + 1, equation.size() - splitIndex - 1);

	if (errorString == "") {
		SwapScreens(false);
		((Graph*)this->subScreen->GetElementByName("graph"))->SetFx(getPostFix(this->rightSide));
	}
}

void EquationAnalizer::SwapScreens(bool toEdit)
{
	if (toEdit != onEdit) {
		onEdit = !onEdit;
		this->subScreen->SwapUiElements(this->otherUiElements);
	}
}

void EquationAnalizer::SetSubScreen(Screen* subScreen)
{
	this->subScreen = subScreen;

	Image* subWindowBackround = new Image(*UiElement::textureMap["SubWindow"], { 0, 0 }, { BASE_SCREEN_W, BASE_SCREEN_H });
	subScreen->AddElement(subWindowBackround);

	auto closesub = [this]() mutable {this->subScreen->SetActive(false); return; };
	auto closeButton = new Button(*UiElement::textureMap["xButton"], closesub, { 100, 15 }, { 100,100 });
	subScreen->AddElement(closeButton);

	auto maximize = [this]() mutable {printf("not yet\n"); return; };
	auto minimizeButton = new Button(*UiElement::textureMap["MMButton"], maximize, { 220, 15 }, { 100,100 });
	subScreen->AddElement(minimizeButton);

	DragBar* dragBar = new DragBar(*UiElement::textureMap["Empty"], subScreen, { 340, 0 }, { BASE_SCREEN_W - 450, 130 });
	subScreen->AddElement(dragBar);

	auto equationTextBox = new TextBox(*UiElement::textureMap["TextBox"], UiElement::baseFont, { BASE_SCREEN_W / 2, BASE_SCREEN_H / 4 * 2 }, { 800 * 1.5, 130 * 1.5 },
		"Enter The Equation", sf::Color::Black, 70);
	equationTextBox->SetName("eqTextBox");
	equationTextBox->SetOrigin(Center);
	subScreen->AddElement(equationTextBox);

	auto saveEqButton = new Button(*UiElement::textureMap["saveThisButton"], [this]() {
		this->LoadFromString(((TextBox*)this->subScreen->GetElementByName("eqTextBox"))->GetText());
		},
		{ BASE_SCREEN_W / 2, BASE_SCREEN_H / 6 * 5 }, { 200 * 4.3, 200 });
	saveEqButton->SetOrigin(Center);
	subScreen->AddElement(saveEqButton);

	otherUiElements.push_back(subWindowBackround);
	otherUiElements.push_back(closeButton);
	otherUiElements.push_back(minimizeButton);
	otherUiElements.push_back(dragBar);

	std::vector<CalcElement> calcVector = {};
	Graph* graphUiElement = new Graph({ BASE_SCREEN_W / 2, BASE_SCREEN_H / 3 * 1.7 }, { 1300, 800 }, calcVector);
	graphUiElement->SetOrigin(Center);
	graphUiElement->SetName("graph");
	otherUiElements.push_back(graphUiElement);
}
