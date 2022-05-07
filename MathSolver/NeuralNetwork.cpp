#include "NeuralNetwork.h"

double GetRand() {
	return (rand() % 2001 - 1000.0) / 1000.0;
}


void RandMat(int n, int m, double* matrix) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			matrix[i * m + j] = GetRand();
		}
	}
}

void SetMat(int n, int m, double* matrix, int val = 0) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			matrix[i * m + j] = val;
		}
	}
}

// change a w matrix with the gradient we found
void SubByG(int n, int m, double* matrix, double* matrixG, double jumpSize, int SetLen) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			matrix[i * m + j] -= (jumpSize / (double)SetLen) * matrixG[i * m + j];
		}
	}
}

double func(double x) {
	return (1 / (1 + exp(-x)));
}

double funcD(double sigX) {
	return sigX * (1 - sigX);
}



NeuralNetwork::NeuralNetwork()
{
	layers[0].resize(NumOfNodesInInput);
	layers[NumOfHiddenLayers + 1].resize(NumOfNodesInOutput);
	bias[NumOfHiddenLayers].resize(NumOfNodesInOutput);
	for (int i = 1; i <= NumOfHiddenLayers; i++)
	{
		layers[i].resize(NumOfNodesInHidden);
		bias[i - 1].resize(NumOfNodesInHidden);
	}
	RandMat(NumOfNodesInHidden, NumOfNodesInInput, *inputW);
	RandMat(NumOfNodesInOutput, NumOfNodesInHidden, *outputW);
	for (int i = 0; i < NumOfHiddenLayers - 1; i++) {
		RandMat(NumOfNodesInHidden, NumOfNodesInHidden, *(hiddenWs[i]));
	}
	for (int i = 0; i <= NumOfHiddenLayers; i++) {
		for (auto& b : bias[i]) {
			b = GetRand();
		}
	}

}

NeuralNetwork::NeuralNetwork(string fileName)
{
	layers[0].resize(NumOfNodesInInput);
	layers[NumOfHiddenLayers + 1].resize(NumOfNodesInOutput);
	bias[NumOfHiddenLayers].resize(NumOfNodesInOutput);
	for (int i = 1; i <= NumOfHiddenLayers; i++)
	{
		layers[i].resize(NumOfNodesInHidden);
		bias[i - 1].resize(NumOfNodesInHidden);
	}

	std::ifstream file(fileName, std::ofstream::binary);
	if (!file) {
		cout << "cant open file" << endl;
		return;
	}

	// mod bias
	for (int i = 0; i <= NumOfHiddenLayers; i++) {
		int len = bias[i].size();
		for (int j = 0; j < len; j++) {
			file.read((char*)&(bias[i][j]), sizeof(double));
		}
	}

	// mod input to first hidden
	for (int i = 0; i < NumOfNodesInHidden; i++) {
		for (int j = 0; j < NumOfNodesInInput; j++) {
			file.read((char*)&(inputW[i][j]), sizeof(double));
		}
	}

	// mod last hidden to output
	for (int i = 0; i < NumOfNodesInOutput; i++) {
		for (int j = 0; j < NumOfNodesInHidden; j++) {
			file.read((char*)&(outputW[i][j]), sizeof(double));
		}
	}

	// mod from hidden to hidden
	for (int k = 0; k < NumOfHiddenLayers - 1; k++) {
		for (int i = 0; i < NumOfNodesInHidden; i++) {
			for (int j = 0; j < NumOfNodesInHidden; j++) {
				file.read((char*)&(hiddenWs[k][i][j]), sizeof(double));
			}
		}
	}
	file.close();
}

int NeuralNetwork::Calc(double* input)
{
	// from input to first hidden
	for (int i = 0; i < NumOfNodesInInput; i++) {
		layers[0][i] = input[i];
	}

	for (int i = 0; i < NumOfNodesInHidden; i++) {
		layers[1][i] = bias[0][i];
		for (int j = 0; j < NumOfNodesInInput; j++) {
			layers[1][i] += layers[0][j] * inputW[i][j];
		}
		layers[1][i] = func(layers[1][i]);
	}

	// from every to hidden 
	for (int k = 0; k < NumOfHiddenLayers - 1; k++) {
		for (int i = 0; i < NumOfNodesInHidden; i++) {
			layers[k + 2][i] = bias[k + 1][i];
			for (int j = 0; j < NumOfNodesInHidden; j++) {
				layers[k + 2][i] += layers[k + 1][j] * hiddenWs[k][i][j];
			}
			layers[k + 2][i] = func(layers[k + 2][i]);
		}
	}

	// from last hidden to output
	for (int i = 0; i < NumOfNodesInOutput; i++) {
		layers[NumOfHiddenLayers + 1][i] = bias[NumOfHiddenLayers][i];
		for (int j = 0; j < NumOfNodesInHidden; j++) {
			layers[NumOfHiddenLayers + 1][i] += layers[NumOfHiddenLayers][j] * outputW[i][j];
		}
		layers[NumOfHiddenLayers + 1][i] = func(layers[NumOfHiddenLayers + 1][i]);
	}

	// find the max output
	int maxn = 0;
	for (int i = 1; i < NumOfNodesInOutput; i++) {
		if (layers[NumOfHiddenLayers + 1][i] > layers[NumOfHiddenLayers + 1][maxn]) {
			maxn = i;
		}
	}
	return maxn;

}

void NeuralNetwork::SaveToFile(string fileName)
{
	std::ofstream file(fileName, std::ofstream::binary);
	if (!file) {
		cout << "cant open file" << endl;
		return;
	}

	// mod bias
	for (int i = 0; i <= NumOfHiddenLayers; i++) {
		int len = bias[i].size();
		for (int j = 0; j < len; j++) {
			file.write((const char*)&(bias[i][j]), sizeof(double));
		}
	}

	// mod input to first hidden
	for (int i = 0; i < NumOfNodesInHidden; i++) {
		for (int j = 0; j < NumOfNodesInInput; j++) {
			file.write((const char*)&(inputW[i][j]), sizeof(double));
		}
	}

	// mod last hidden to output
	for (int i = 0; i < NumOfNodesInOutput; i++) {
		for (int j = 0; j < NumOfNodesInHidden; j++) {
			file.write((const char*)&(outputW[i][j]), sizeof(double));
		}
	}

	// mod from hidden to hidden
	for (int k = 0; k < NumOfHiddenLayers - 1; k++) {
		for (int i = 0; i < NumOfNodesInHidden; i++) {
			for (int j = 0; j < NumOfNodesInHidden; j++) {
				file.write((const char*)&(hiddenWs[k][i][j]), sizeof(double));
			}
		}
	}
	file.close();
}

void NeuralNetwork::BackProp(vector<double*> input, vector<int> label, double jumpSize)
{
	double inputWG[NumOfNodesInHidden][NumOfNodesInInput];
	double outputWG[NumOfNodesInOutput][NumOfNodesInHidden];
	HiddenW hiddenWsG[NumOfHiddenLayers - 1];
	vector<double> biasG[NumOfHiddenLayers + 1];
	vector<double> error[NumOfHiddenLayers + 2];
	int SetLen = input.size();

	error[0].resize(NumOfNodesInInput);
	error[NumOfHiddenLayers + 1].resize(NumOfNodesInOutput);
	biasG[NumOfHiddenLayers].resize(NumOfNodesInOutput);

	// set the size of error and biasG vectors
	for (int i = 1; i <= NumOfHiddenLayers; i++)
	{
		error[i].resize(NumOfNodesInHidden);
		biasG[i - 1].resize(NumOfNodesInHidden);
	}

	// set all the gradient arrs to 0
	SetMat(NumOfNodesInHidden, NumOfNodesInInput, *inputWG);
	SetMat(NumOfNodesInOutput, NumOfNodesInHidden, *outputWG);
	for (int i = 0; i < NumOfHiddenLayers - 1; i++) {
		SetMat(NumOfNodesInHidden, NumOfNodesInHidden, *(hiddenWsG[i]));
	}
	for (int i = 0; i <= NumOfHiddenLayers; i++) {
		for (auto& b : biasG[i]) {
			b = 0;
		}
	}

	// reset the error
	for (int t = 0; t < SetLen; t++) {
		//cout << label[t] << "l" << endl;
		//cout << Calc(input[t]) << "c" << endl;
		Calc(input[t]);
		for (int p = 0; p < NumOfHiddenLayers + 2; p++) {
			for (auto& e : error[p]) {
				e = 0;
			}
		}
		for (int j = 0; j < NumOfNodesInOutput; j++) {
			// error(L,j) = Dsig(Z(L,j)) * (a(L,j) - y(j))
			error[NumOfHiddenLayers + 1][j] = funcD(layers[NumOfHiddenLayers + 1][j]) *
				(layers[NumOfHiddenLayers + 1][j] - (label[t] == j));
			for (int k = 0; k < NumOfNodesInHidden; k++) {
				// error(l, k) = sumForEveryJ(w(l+1,j,k) * error(j, l+1) * Dsig(z(l, k)))
				error[NumOfHiddenLayers][k] += outputW[j][k] * error[NumOfHiddenLayers + 1][j]
					* funcD(layers[NumOfHiddenLayers][k]);
				// dc/dw(l,j,k) = a(l-1, k) * error(l, j)
				outputWG[j][k] += layers[NumOfHiddenLayers][k] * error[NumOfHiddenLayers + 1][j];
			}
			// dc/db(l, j) = error(l,j)
			biasG[NumOfHiddenLayers][j] += error[NumOfHiddenLayers + 1][j];
		}

		// calc back for all hiddens
		for (int ly = NumOfHiddenLayers - 2; ly >= 0; ly--) {
			int l = ly + 2;
			for (int j = 0; j < NumOfNodesInHidden; j++) {
				for (int k = 0; k < NumOfNodesInHidden; k++) {
					// error(l, k) = sumForAllJ(W(l+1, j, k)*error(l+1, j)*Dsig(z(l, k)))
					error[l - 1][k] += hiddenWs[ly][j][k] * error[l][j]
						* funcD(layers[l - 1][k]);
					// dc/dw(l,j,k) = a(l-1, k) * error(l, j)
					hiddenWsG[ly][j][k] += layers[l - 1][k] * error[l][j];
				}
				// dc/db(l, j) = error(l,j)
				biasG[l - 1][j] += error[l][j];
			}
		}

		// calc for input
		for (int j = 0; j < NumOfNodesInHidden; j++) {
			for (int k = 0; k < NumOfNodesInInput; k++) {
				// dc/dw(l,j,k) = a(l-1, k) * error(l, j)
				inputWG[j][k] += layers[0][k] * error[1][j];
			}
			// dc/db(l, j) = error(l,j)
			biasG[0][j] += error[1][j];
		}
	}

	// change all the w matrixs with the gradients we found
	SubByG(NumOfNodesInHidden, NumOfNodesInInput, *inputW, *inputWG, jumpSize, SetLen);
	SubByG(NumOfNodesInOutput, NumOfNodesInHidden, *outputW, *outputWG, jumpSize, SetLen);
	for (int i = 0; i < NumOfHiddenLayers - 1; i++) {
		SubByG(NumOfNodesInHidden, NumOfNodesInHidden, *(hiddenWs[i]), *(hiddenWsG[i]), jumpSize, SetLen);
	}
	// change the bias with the gradient we found
	for (int i = 0; i <= NumOfHiddenLayers; i++) {
		int bLen = bias[i].size();
		for (int j = 0; j < bLen; j++) {
			bias[i][j] -= (jumpSize / (double)SetLen) * biasG[i][j];
		}
	}
}


void randomize(vector<double*>& input, vector<int>& label) {
	int s = input.size();
	for (int i = 0; i < s; i++) {
		int k = rand() % s;
		swap(input[i], input[k]);
		swap(label[i], label[k]);
	}
}

void NeuralNetwork::Learn(vector<double*>& input, vector<int>& label, double jumpSize)
{
	int totalLen = input.size();
	vector<double*> trainS;
	vector<int> labelS;
	for (int t = 0; t < TimeTOGoOverTrain; t++) {
		randomize(input, label);
		for (int i = 0; i < totalLen / SetSize; i++) {
			for (int j = 0; j < SetSize; j++) {
				trainS.push_back(input[SetSize * i + j]);
				labelS.push_back(label[SetSize * i + j]);
			}
			BackProp(trainS, labelS, jumpSize);
			trainS.clear();
			labelS.clear();
		}
		cout << t << endl;
	}
}

void NeuralNetwork::Test(vector<double*>& input, vector<int>& label)
{
	int len = input.size();
	int cor = 0;
	for (int i = 0; i < len; i++) {
		if (Calc(input[i]) == label[i])
			cor++;
	}
	cout << (double)cor / len << endl;
}
