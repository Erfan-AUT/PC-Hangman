// HangmanPage.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <string>
#include <vector>
#include <Windows.h>

using namespace std;

COORD GetConsoleCursorPosition(HANDLE hConsoleOutput)
{
	CONSOLE_SCREEN_BUFFER_INFO cbsi;
	if (GetConsoleScreenBufferInfo(hConsoleOutput, &cbsi))
	{
		return cbsi.dwCursorPosition;
	}
	else
	{
		// The function failed. Call GetLastError() for details.
		COORD invalid = { 0, 0 };
		return invalid;
	}
}

BOOL setxy(int x, int y)
{
	COORD c = { x,y };
	return SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void DrawRect(int x, int y, int width, int height, int curPosX = 0, int curPosY = 0)
{
	setxy(x, y); cout << char(201);
	for (int i = 1; i < width; i++)
		cout << char(205);
	cout << char(187);
	setxy(x, height + y);
	cout << char(200);
	for (int i = 1; i < width; i++)
		cout << char(205);
	cout << char(188);
	for (int i = y + 1; i < height + y; i++)
	{
		setxy(x, i);
		cout << char(186);
		setxy(x + width, i);
		cout << char(186);
	}
	setxy(curPosX, curPosY);
}

void DrawingTheGuy(short chances)
{

}

void ReadNthLine(string &line, ifstream &file, int n)
{
	int b = 0; char c;
	while (b != n - 1)
	{
		c = file.get();
		if (c == '\n')
			b++;
	}
	file >> line;
}

int main()
{
	string playerName, line, topicsListAddress = "AVAILABLE_TOPICS.txt", selectedTopicAddress;
	cout << "Enter your name:" << endl;
	cin >> playerName;
	char gameType;
	int showCounter = 1, realCounter = 0, whichTopicNumber;
	cout << "Type r for resuming your previous game, n for playing a new game" << endl;
	cin >> gameType;
	while ((gameType != 'r') || (gameType != 'c'))
	{
		cout << "Wrong input, please specify your orders again." << endl;
		cin >> gameType;
	}
	ifstream listOfTopics(topicsListAddress);
	cout << "Please choose the number of one of the following subjects:" << endl;
	while (!listOfTopics.eof())
	{
		getline(listOfTopics, line);
		cout << showCounter << ":  " << line << endl;
		showCounter++;
	}
	cin >> whichTopicNumber;
	ReadNthLine(selectedTopicAddress, listOfTopics, whichTopicNumber);
	ifstream selectedTopicFile(selectedTopicAddress);
	vector <string> includedWords;
	while (!selectedTopicFile.eof())
	{
		getline(selectedTopicFile, line);
		includedWords.push_back(line);
	}
	int randomWordNumber = rand() % includedWords.size();
	cout << endl << "Okay! Game will start in 3..2.." << endl;
	system("cls");
	string currentWord = includedWords[randomWordNumber];
	includedWords.erase(includedWords.begin() + randomWordNumber);
	cout << "Now, the word consists of these letters, are you gonna be able to find them?" << endl << "  ";
	for (int i = 0; i < currentWord.length(); i++)
		cout << "_ ";
	cout << endl << endl << "Now, LET THE GAMES BEGIN!" << endl;

    return 0;
}

