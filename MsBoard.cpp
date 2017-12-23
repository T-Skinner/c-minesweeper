#include "MsBoard.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <ctime>

template<typename Out>
void split(const std::string &s, char delim, Out result) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		*(result++) = item;
	}
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, std::back_inserter(elems));
	return elems;
}


std::vector<std::string> split(const string &s, const string &delims) {
	std::stringstream stringStream(s);
	std::string line;
	vector<string> wordVector;
	while (std::getline(stringStream, line))
	{
		std::size_t prev = 0, pos;
		while ((pos = line.find_first_of(" ';", prev)) != std::string::npos)
		{
			if (pos > prev)
				wordVector.push_back(line.substr(prev, pos - prev));
			prev = pos + 1;
		}
		if (prev < line.length())
			wordVector.push_back(line.substr(prev, std::string::npos));
	}
	return wordVector;
}


MsBoard::MsBoard(int width, int height, double mineDensity)
{
	srand(time(NULL));
	mineField = vector<vector<MsBoardPosition>>(height, vector<MsBoardPosition>(width, MsBoardPosition()));
	int positions = width * height;
	int mines = positions * mineDensity;
	while (mines > 0) {
		int x = rand() % width, y = rand() % height;
		while (mineField[y][x].hasMine) {
			x = rand() % width, y = rand() % height;
		}
		mineField[y][x].hasMine = true;
		mines--;
	}
	for (int i = 0; i < mineField.size(); i++) {
		for (int j = 0; j < mineField[i].size(); j++) {
			int surrounding = 0;
			for (int x = -1; x <= 1; x++) {
				for (int y = -1; y <= 1; y++) {
					int cx = j + x, cy = i + y;
					if ((x == 0 && y == 0) || cx < 0 || cy < 0 || cx >= width || cy >= height) {
						continue;
					}
					if (mineField[cy][cx].hasMine) surrounding++;
				}
			}
			mineField[i][j].adjacentMines = surrounding;
		}
	}
}

bool MsBoard::isWinningConfiguration() {
	bool won = true;
	for (int i = 0; i < mineField.size(); i++) {
		for (int j = 0; j < mineField[i].size(); j++) {
			MsBoardPosition &pos = mineField[i][j];
			if (!pos.hasMine && !pos.exposed) won = false;
		}
	}
	return won;
}

void MsBoard::startGame() {
	bool lost = false, won = false;
	while (!lost && !won) {
		system("cls");
		printBoard();
		cout << endl;
		cout << "Please enter location to play. Origin is top left. Input format: X Y" << endl;
		string inp;
		getline(cin, inp);
		vector<string> inputs = split(inp, " \n");
		//for (auto it = inputs.begin(); it != inputs.end(); it++) {

		//}
		int inputX = stoi(inputs[0]);
		int inputY = stoi(inputs[1]);
		RevealResult res = revealPos(inputX, inputY);
		if (res == MINE) lost = true;
		else won = isWinningConfiguration();
	}
	system("cls");
	printBoard();
	if (lost == true) {
		cout << "You lose.";
		cin >> lost;
	}
	if (won == true) {
		cout << "You won.";
		cin >> lost;
	}
	return;
}

void MsBoard::printBoard() {
	for (int i = 0; i < mineField.size(); i++) {
		for (int j = 0; j < mineField[i].size(); j++) {
			MsBoardPosition &pos = mineField[i][j];
			if (pos.exposed) {
				if (pos.hasMine)
					cout << " X ";
				else if (pos.adjacentMines > 0)
					cout << " " << pos.adjacentMines << " ";
				else
					cout << "   ";
			}
			else {
				cout << " - ";
			}
		}
		cout << endl;
	}
}

RevealResult MsBoard::revealPos(int x, int y) {
	MsBoardPosition &pos = mineField[y][x];
	//if (pos.exposed) return;
	pos.exposed = true;
	if (pos.hasMine) { //expose all mines if 1 mine is found
		for (int i = 0; i < mineField.size(); i++) {
			for (int j = 0; j < mineField[i].size(); j++) {
				if (mineField[i][j].hasMine) mineField[i][j].exposed = true;
			}
		}
		return MINE;
	}
	for (int dx = -1; dx <= 1; dx++) {
		for (int dy = -1; dy <= 1; dy++) {
			int cx = dx + x, cy = dy + y;
			if ((dx == 0 && dy == 0) || (dx == dy) || (dx == -dy) || cx < 0 || cy < 0 || cx >= mineField[y].size() || cy >= mineField.size()) {
				continue;
			}
			if (mineField[cy][cx].hasMine || mineField[cy][cx].exposed) continue;
			revealPos(cx, cy);
		}
	}
	return EMPTY;
}

MsBoard::~MsBoard()
{
}
