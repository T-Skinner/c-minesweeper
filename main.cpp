#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include "MsBoard.h"

int main()
{
	MsBoard board(8, 8, 0.5);
	board.startGame();

    return 0;
}
