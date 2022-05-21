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
				if (ele.Element.op == '-' && i < stringLen - 1 && ((s[i + 1] <= '9' && s[i + 1] >= '0') || s[i + 1] == 'x')) {
					mulBy = -1;
					continue;
				}
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

std::list<std::pair<double, double>> MulEq(std::list<std::pair<double, double>>& eq1, std::list<std::pair<double, double>>& eq2) {
	std::list<std::pair<double, double>> newList;
	for (auto& ex1 : eq1) {
		for (auto& ex2 : eq2) {
			newList.push_back({ex1.first * ex2.first, ex1.second + ex2.second});
		}
	}
	return newList;
}

std::list<std::pair<double, double>> PostFixToEq(std::vector<CalcElement> postFix) {
	std::stack<std::list<std::pair<double, double>>> equationStack;

	// go over the postfix
	for (auto part : postFix) {
		if (part.IsNum) {
			// push num: num*X*0
			equationStack.push({ {part.Element.num, 0} });
		}
		else {
			if (part.Element.op == 'x') {
				// push x: 1*x^1
				equationStack.push({ {1, 1} });
			}
			else {
				if (part.Element.op == '^') {
					if (equationStack.top().size() == 1 && equationStack.top().begin()->second == 0 && int(equationStack.top().begin()->first) - equationStack.top().begin()->first == 0) {
						// check the ^
						int count = equationStack.top().begin()->first;
						equationStack.pop();
						if (count == 0) {
							equationStack.pop();
							equationStack.push({ {1,0} });
							continue;
						}
						auto oldlist = equationStack.top();
						std::list<std::pair<double, double>> newlist = equationStack.top();
						equationStack.pop();

						// mul new one by it self
						for (int i = 1; i < count; i++) {
							newlist = MulEq(oldlist, newlist);
						}
						equationStack.push(newlist);
					}
					else {
						throw "can't solve this";
					}
				}
				else if (part.Element.op == '*') {
					// do mul
					auto eq1 = equationStack.top();
					equationStack.pop();
					auto eq2 = equationStack.top();
					equationStack.pop();
					equationStack.push(MulEq(eq1, eq2));
				}
				else if (part.Element.op == '/') {
					// do div only if divisor is num
					if (equationStack.top().size() == 1 && equationStack.top().begin()->second == 0) {
						int divBy = equationStack.top().begin()->first;
						equationStack.pop();
						for (auto& eq : equationStack.top()) {
							eq.first /= divBy;
						}
					}
					else {
						throw "can't solve this";
					}
				}
				else if (part.Element.op == '+' or part.Element.op == '-') {
					auto eq1 = equationStack.top();
					equationStack.pop();
					auto eq2 = equationStack.top();
					equationStack.pop();
					if (eq1.size() == 1 && eq1.begin()->second == 0 && eq2.size() == 1 && eq2.begin()->second == 0) {
						if(part.Element.op == '+')
							equationStack.push({ { eq1.begin()->first + eq2.begin()->first, 0} });
						else
							equationStack.push({ { eq2.begin()->first - eq1.begin()->first, 0} });
					}
					else {
						if (part.Element.op == '-') {
							// mul by -1
							for (auto& eq : equationStack.top()) {
								eq.first *= -1;
							}
						}
						// do addition
						eq2.insert(eq2.end(), eq1.begin(), eq1.end());
						equationStack.push(eq2);
					}
				}
				else {
					throw "can't solve this";
				}
			}
		}
	}

	return equationStack.top();
}

void addToEq(std::map<int, double>& fullEq, pair<double, double> item, int mulBy) {
	if (item.second - int(item.second) != 0)
		throw "can't solvet this";
	int key = item.second;
	auto mapPointer = fullEq.find(key);
	if (mapPointer == fullEq.end()) {
		fullEq[key] = item.first * mulBy;
	}
	else {
		fullEq[key] += item.first * mulBy;
	}
}

std::string SolveForX (std::string leftSide, std::string rightSide) {
	// stack of list of pair - the pair represents (first)*X^(second) where first and second are the pair
	try {
		auto rightEq = PostFixToEq(getPostFix(rightSide));
		auto leftEq = PostFixToEq(getPostFix(leftSide));
		std::map<int, double> fullEq;
		for (auto i : rightEq) {
			addToEq(fullEq, i, 1);
		}
		for (auto i : leftEq) {
			addToEq(fullEq, i, -1);
		}
		for (auto ex : fullEq) {
			std::cout << ex.first << "x^" << ex.second << " + ";
		}
		return "";
	}
	catch(...){
		std::cout << "can't solve this" << std::endl;
		return "";
	}


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
	bool hasX = false, hasY = false;
	int splitIndex = -1;
	for (int i = 0; i < equation.size(); i++) {
		if (std::find(okCharsFromEqString.begin(), okCharsFromEqString.end(), equation[i]) == okCharsFromEqString.end()) {
			errorString = "Equation is wrong";
			break;
		}

		hasX = hasX || (equation[i] == 'x');
		hasY = hasY || (equation[i] == 'y');

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
		if (leftSide == "y") {
			SwapScreens(false);
			((Graph*)this->subScreen->GetElementByName("graph"))->SetFx(getPostFix(this->rightSide));
		}
		else if (!hasX && !hasY && rightSide == "") {

		}
		else if(!hasY) {
			SolveForX(leftSide, rightSide);
		}
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
	Graph* graphUiElement = new Graph({ BASE_SCREEN_W / 2, BASE_SCREEN_H / 3 * 1.7 }, { 1500, 900 }, calcVector);
	graphUiElement->SetOrigin(Center);
	graphUiElement->SetName("graph");
	otherUiElements.push_back(graphUiElement);
}
