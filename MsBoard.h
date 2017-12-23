#pragma once
#include <vector>

using namespace std;

struct MsBoardPosition {
	bool hasMine;
	bool exposed;
	int adjacentMines;
};

enum RevealResult {MINE, EMPTY};

class MsBoard
{
public:
	MsBoard(int width, int height, double mineDensity);
	void printBoard();
	RevealResult revealPos(int x, int y);
	void startGame();
	~MsBoard();
private:
	vector<vector<MsBoardPosition>> mineField;
	bool isWinningConfiguration();

};

