#include "EquationAnalizer.h"

std::vector<CalcElement> getPostFix(string s) {
	const int NUM = 1, X = 2, OPER = 3;
	std::stack<CalcElement> opStack;
	std::vector<CalcElement> result;
	int stringLen = s.size();
	int last = 0;
	int mulBy = 1;
	for (int i = 0; i < stringLen; i++) {
		// if there is a y, exit
		if (s[i] == 'y') {
			throw std::invalid_argument("Can't have y here");
		}
		// if it is a digit
		if (s[i] <= '9' && s[i] >= '0') {
			// digit can't come after x
			if (last == X) {
				throw std::invalid_argument("Equation is wrong");
			}

			// get the digit
			CalcElement ele;
			ele.Element.num = 0;
			ele.IsNum = true;

			// get the num
			while (i < stringLen && s[i] <= '9' && s[i] >= '0') {
				ele.Element.num *= 10;
				ele.Element.num += s[i] - '0';
				i++;
			}

			// if the number needs to be negative
			ele.Element.num *= mulBy;
			mulBy = 1;

			// get back to the last char
			i--;
			result.push_back(ele);
			last = NUM;
		}
		else if (s[i] == 'x' && last != NUM) {
			// for x

			// x can't come after x
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
			// for opetator
			CalcElement ele;
			ele.IsNum = false;
			if (s[i] == '(') {
				// add ( if num or x came before
				if (last == X || last == NUM) {
					ele.Element.op = '*';
					i--;
				}
				else {
					// add the ( to the stack
					ele.Element.op = '(';
					opStack.push(ele);
					last = 0;
					continue;
				}
			}
			else if (s[i] == ')') {

				// loop until we find '('
				while (!opStack.empty() && opStack.top().Element.op != '(') {
					result.push_back(opStack.top());
					opStack.pop();
				}
				// pop from the stack
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
					// do + or -
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
							for (auto& eq : eq1) {
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

double CalcFunctionFromDict(std::map<int, double>& eq, double x, bool derivative = false) {
	double fx = 0;
	for (auto eqPart : eq) {
		if (derivative) {
			// calc for the f'(x)
			fx += eqPart.second * eqPart.first * std::pow(x, eqPart.first - 1);
		}
		else {
			// calc for the f(x)
			fx += eqPart.second * std::pow(x, eqPart.first);
		}
	}
	return fx;
}

std::pair<bool, double> NewtonMethod(std::map<int, double>& eq, double x, int maxIterations) {
	for (int i = 0; i < maxIterations; i++) {
		// get y value (f(x))
		double fx = CalcFunctionFromDict(eq, x);

		// if the fx is close enough to 0
		if (std::abs(fx) < NEWTON_MIN_VALID) {
			return { true, x };
		}

		// get y' value(f'(x))
		double fxd = CalcFunctionFromDict(eq, x, true);

		// make sure the f'(x) is not 0
		if (fxd == 0)
			return { false, 0 };

		// get the next x
		x -= fx / fxd;
	}
	return { false, 0 };
}

std::pair<int, std::pair<double, double>> QuadraticFormula(std::map<int, double>& eq) {
	double a = (eq.find(2) == eq.end()) ? 0 : eq[2];
	double b = (eq.find(1) == eq.end()) ? 0 : eq[1];
	double c = (eq.find(0) == eq.end()) ? 0 : eq[0];
	
	if (b == 0 && c == 0 && a == 0)
		return { -1, {0,0} };
	else if (b == 0 && a == 0)
		return { 0, {0,0} };
	else if (a == 0)
		return { 1, {-c / b ,0} };

	int d = b * b - 4 * a * c;

	if (d < 0)
		return { 0, {0, 0} };
	else if (d == 0)
		return { 1, {-b / (2 * a), 0} };
	else
		return { 2, {(-b + std::sqrt(d)) / (2 * a), (-b - std::sqrt(d)) / (2 * a)} };
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
		
		// the largest element
		if ((--(fullEq.end()))->first <= 2) {
			auto ans = QuadraticFormula(fullEq);
			if(ans.first == -1)
				return "Every x is a solotion";
			else if (ans.first == 0)
				return "No solotion for x";
			else if (ans.first == 1)
				return "x = " + std::to_string(ans.second.first);
			else
				return "x1 = " + std::to_string(ans.second.first) + "\nx2 = " + std::to_string(ans.second.second);

		}
		else {
			for (int i = 0; i < NEWTON_TRYS; i++) {
				auto sol = NewtonMethod(fullEq, rand() % 201 - 100, NEWTON_ITERATIONS);
				if (sol.first) {
					return "x = " + std::to_string(sol.second);
				}
			}
			return "can't find solotion for x";
		}
	}
	catch(...){
		return "can't solve this kind of equation";
	}


}

EquationAnalizer::EquationAnalizer()
{
	network = new NeuralNetwork({
	new ConvolutionalLayer(45, 45, 1, 3, 3),
	new Sigmoid(43 * 43 * 3),
	new ConvolutionalLayer(43, 43, 3, 4, 2),
	new Sigmoid(40 * 40 * 2),
	new FullyConnectedLayer(40 * 40 * 2, 40),
	new Sigmoid(40),
	new FullyConnectedLayer(40, 14),
	new Sigmoid(14),
		}, 2025, 14, NNUploadFile);
	this->subScreen = nullptr;
	onEdit = true;
}

#define CenterX(rect) ((rect.left+(rect.left+rect.width))/2.0)
#define CenterY(rect) ((rect.top+(rect.top+rect.height))/2.0)

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

std::pair<int, pair<char, double>> GetCharFromResults(vector<pair<char, double>> nnResults, char find) {
	int resLen = nnResults.size();
	for (int i = 0; i < resLen; i++) {
		if (nnResults[i].first == find) {
			return { i, nnResults[i] };
		}
	}
	return { -1, {-1, -1} };
}

std::string getEqString(std::vector<std::pair<sf::IntRect, double*>>& images, vector<vector<pair<char, double>>>& nnResults) {
	std::string eq = "";

	int len = images.size();

	for (int i = 0; i < len; i++) {
		if (nnResults[i][0].first == '-') {
			int k = i;
			std::vector<std::pair<sf::IntRect, double*>> downImages, upImages;
			vector<vector<pair<char, double>>> downNnResults, upNnResults;
			while (++i < len) {
				if (images[i].first.left > images[k].first.left + images[k].first.width)
					break;
				if (images[i].first.top < images[k].first.top) {
					upImages.push_back(images[i]);
					upNnResults.push_back(nnResults[i]);
				}
				else {
					downImages.push_back(images[i]);
					downNnResults.push_back(nnResults[i]);
				}
			}
			i--;

			if (i == k)
				eq += '-';
			else {
				eq += "(" + getEqString(upImages, upNnResults) + ")/(" + getEqString(downImages, downNnResults) + ")";
			}
		}
		else {
			eq += (nnResults[i][0].first == 'y') ? 'x' : nnResults[i][0].first;

			if (i + 1 < len) {
				if (CenterY(images[i + 1].first) < images[i].first.top &&
					0.75 * std::max(images[i].first.width, images[i].first.height) > std::max(images[i + 1].first.width, images[i + 1].first.height)) {
					eq += '^';
				}
			}
		}
	}

	return eq;
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
			auto temp = network->FeedForword(images[i].second);
			for (int j = 0; j < temp.size(); j++) {
				nnResult.push_back({symbols[j], temp[j]});
			}
			std::sort(nnResult.begin(), nnResult.end(), ResultCmp);
			nnResults.push_back(nnResult);
		}

		double bestScore = 50;
		size_t bestIndex = -1;
		auto lastFindRes = GetCharFromResults(nnResults[0], '-');
		for(int i = 1; i < imageCount; i++) {
			auto findRes = GetCharFromResults(nnResults[i], '-');
			if (findRes.first < 4 && lastFindRes.first < 4) {
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

		if (bestIndex == 2)
			leftSide += nnResults[0][0].first;
		else {
			auto tempImg = std::vector(images.begin(), images.begin() + (bestIndex - 1));
			auto tempRes = std::vector(nnResults.begin(), nnResults.begin() + (bestIndex - 1));
			leftSide = getEqString(tempImg, tempRes);
		}

		auto tempImg = std::vector(images.begin() + (bestIndex + 1), images.end());
		auto tempRes = std::vector(nnResults.begin() + (bestIndex + 1), nnResults.end());
		rightSide = getEqString(tempImg, tempRes);

		//for (int i = 0; i < bestIndex - 1; i++) {
		//	if(bestIndex == 2)
		//		leftSide += nnResults[i][0].first;
		//	else
		//		leftSide += (nnResults[i][0].first == 'y')? nnResults[i][1].first: nnResults[i][0].first;

		//	if (i + 1 < bestIndex) {
		//		if (CenterY(images[i + 1].first) < images[i].first.top &&
		//			0.75 * std::max(images[i].first.width, images[i].first.height) > std::max(images[i + 1].first.width, images[i + 1].first.height)) {
		//			leftSide += '^';
		//		}
		//	}
		//}
		//for (int i = bestIndex + 1; i < imageCount; i++) {
		//	rightSide += (nnResults[i][0].first == 'y') ? 'x' : nnResults[i][0].first;

		//	if (i + 1 < imageCount) {
		//		if (CenterY(images[i + 1].first) < images[i].first.top &&
		//			0.75 * std::max(images[i].first.width, images[i].first.height) > std::max(images[i + 1].first.width, images[i + 1].first.height)) {
		//			rightSide += '^';
		//		}
		//	}
		//}
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

	this->leftSide = equation.substr(0, splitIndex);
	this->rightSide = equation.substr(splitIndex + 1, equation.size() - splitIndex - 1);

	if (errorString == "") {
		SwapScreens(false);
		if (leftSide == "y" && this->rightSide != "") {
			this->subScreen->GetElementByName("graph")->SetActive(true);
			this->subScreen->GetElementByName("solText")->SetActive(false);
			((Graph*)this->subScreen->GetElementByName("graph"))->SetFx(getPostFix(this->rightSide));
		}
		else if (!hasX && !hasY && this->rightSide == "" && this->leftSide != "") {
			this->subScreen->GetElementByName("graph")->SetActive(false);
			this->subScreen->GetElementByName("solText")->SetActive(true);
		}
		else if(!hasY && this->rightSide != "" && this->leftSide != "") {
			this->subScreen->GetElementByName("graph")->SetActive(false);
			this->subScreen->GetElementByName("solText")->SetActive(true);
			((TextBox*)this->subScreen->GetElementByName("solText"))->SetString(SolveForX(this->leftSide, this->rightSide));
		}
		else {
			this->subScreen->GetElementByName("graph")->SetActive(false);
			this->subScreen->GetElementByName("solText")->SetActive(true);
			((TextBox*)this->subScreen->GetElementByName("solText"))->SetString("Can't solve this kind of thing");
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

	auto maximize = [this, subScreen]() mutable {
		static sf::FloatRect subPort = {0,0,1,1};
		sf::FloatRect temp = subScreen->GetPort();
		if (subPort.height == 1 || subPort.width == 1) {
			if (temp.width < temp.height) {
				subPort.height = 1;
				subPort.width = temp.width / temp.height;
				subPort.top = 0;
				subPort.left = (1 - temp.width / temp.height) / 2;
			}
			else {
				subPort.width = 1;
				subPort.height = temp.height / temp.width;
				subPort.left = 0;
				subPort.top = (1 - temp.height / temp.width) / 2;
			}
		}
		subScreen->SetPort(subPort);
		subPort = temp;
	};
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

	auto solotionTextBox = new TextBox(*UiElement::textureMap["Empty"], UiElement::baseFont, { BASE_SCREEN_W / 2, BASE_SCREEN_H / 2}, { 800 * 1.5, 130 * 1.5 },
		"Enter The Equation", sf::Color::Red, 100);
	solotionTextBox->SetName("solText");
	solotionTextBox->SetOrigin(Center);
	otherUiElements.push_back(solotionTextBox);
}
