// HangmanPage.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <Windows.h>
#include <ctime>

using namespace std;

bool didGiveUpOnTheGame = 0;
string usedTopicAddress = "";

// The Way this program saves player data is as follows. in the file of playername.txt we have:
// FirstLine : Previous Score.
// SecondLine : Formerly Played Subjects by name.

COORD GetConsoleCursorPosition(HANDLE hConsoleOutput)
{
	CONSOLE_SCREEN_BUFFER_INFO cbsi;
	if (GetConsoleScreenBufferInfo(hConsoleOutput, &cbsi))
		return cbsi.dwCursorPosition;
	else
	{
		// The function failed. Call GetLastError() for details.
		COORD invalid = { 0, 0 };
		return invalid;
	}
}

BOOL setxy(short x, short y)
{
	COORD c = { x,y };
	return SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void DrawRect(int x, int y, int width, int height)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD xy = GetConsoleCursorPosition(h);
	short curPosX = xy.X, curPosY = xy.Y;
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

void DrawingTheGuyPlusWrongGuesses(short numberOfWrongGuesses)
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

int returnPreviousTotalScore(string playerName, char gameType)
{
	if (gameType == 'n')
		return 0;
	ifstream playerFile(playerName + ".txt");
	int n;
	playerFile >> n;
	return n;
}

vector <string> GenerateUnplayedSubjects(string playerName, char gameType)
{
	vector <string> gonnaReturn;
	string topicsListAddress = "AVAILABLE_TOPICS.txt", line;
	int showCounter = 1, gonnaRemove = 0;
	ifstream listOfTopics(topicsListAddress);
	while (!listOfTopics.eof())
	{
		getline(listOfTopics, line);
		gonnaReturn.push_back(line);
	}
	if (gameType == 'r')
	{
		ifstream playerFile(playerName + ".txt");
		getline(playerFile, line);
		while (!playerFile.eof())
		{
			playerFile >> line;
			for (string topics : gonnaReturn)
			{
				if (line == topics)
					gonnaReturn.erase(gonnaReturn.begin() + gonnaRemove);
				gonnaRemove++;
			}
		}
	}
	for (string topics : gonnaReturn)
	{
		cout << showCounter << ":  " << topics << endl;
		showCounter++;
	}
	return gonnaReturn;
}

void PlayOneSubjectUntilItsDone(string &playerName, char &gameType, vector<string> &includedWords, int &totalScore)
{
	string line;
	int randomWordNumber = rand() % includedWords.size();
	cout << endl << "Okay! Game will start in 3..2.." << endl;
	system("cls");
	string currentWord = includedWords[randomWordNumber];
	includedWords.erase(includedWords.begin() + randomWordNumber);
	cout << "Now, the word consists of these letters, are you gonna be able to find them?" << endl << "  ";
	for (int i = 0; i < currentWord.length(); i++)
		cout << "_ ";
	cout << endl << endl << "Now, LET THE GAMES BEGIN!" << endl;
	short numberOfWrongGuesses = 0, numberofRightGuesses = 0;
	int currentScore = 0;
	char currentGuess;
	bool hasGuessedRight;
	string whatToShow = "";
	while (numberOfWrongGuesses < 5)
	{
		hasGuessedRight = 0;
		cout << "Enter your guess: ";
		cin >> currentGuess;
		if (currentGuess != 'Q')
		{
			for (char letter : currentWord)
			{
				if (letter == currentGuess)
				{
					whatToShow += letter + " ";
					hasGuessedRight = 1;
					numberofRightGuesses++;
				}
				else
					whatToShow += "_ ";
			}
			cout << whatToShow << endl;
			if (!hasGuessedRight)
				numberOfWrongGuesses++;
			if (numberofRightGuesses == currentWord.length())
			{
				currentScore = 3 * currentWord.length() - numberOfWrongGuesses;
				cout << "CONGRATS! YOU HAVE FIGURED THE WORD OUT!" << endl;
				break;
			}
			DrawingTheGuyPlusWrongGuesses(numberOfWrongGuesses);
		}
		else
		{
			didGiveUpOnTheGame = 1;
			return;
		}
	}
	totalScore += currentScore;
}

double PlayCalcScoreOfOneSubject(string playerName, char gameType)
{
	time_t starTimer = time(0);
	string line, selectedTopicAddress, checkNotQ;
	cout << "Please choose the number of one of the following subjects:" << endl;
	vector <string> listOfUnplayedSubjects = GenerateUnplayedSubjects(playerName, gameType);
	int whichTopicNumber;
	cin >> checkNotQ;
	if (checkNotQ != "Q")
	{
		stringstream sCheckNotQ(checkNotQ);
		sCheckNotQ >> whichTopicNumber;
		selectedTopicAddress = listOfUnplayedSubjects[whichTopicNumber];
		usedTopicAddress = selectedTopicAddress;
		listOfUnplayedSubjects.clear();
		ifstream selectedTopicFile(selectedTopicAddress);
		vector <string> includedWords;
		while (!selectedTopicFile.eof())
		{
			selectedTopicFile >> line;
			includedWords.push_back(line);
		}
		int totalScore = returnPreviousTotalScore(playerName, gameType);
		while (!includedWords.empty())
			PlayOneSubjectUntilItsDone(playerName, gameType, includedWords, totalScore);
		//ReadNthLine(selectedTopicAddress, listOfTopics, whichTopicNumber);
		time_t endTimer = time(0);
		double finalScore = (double)(totalScore / difftime(endTimer, starTimer));
		return finalScore;
	}
	else
	{
		didGiveUpOnTheGame = 1;
		return -1;
	}
}

void SavingShit()
{

}

void GeneralGamePlay(string playerName, char gameType)
{
	do {
		double totalScore = PlayCalcScoreOfOneSubject(playerName, gameType);
		if (gameType == 'r')
		{
			ofstream playerFile;
			playerFile.open(playerName + ".txt", ios_base::app);
			playerFile << usedTopicAddress;
		}
		else
		{
			// Needs a full eval when it's specified.
		}
		cout << "Here's your total score on this subject: " << totalScore << endl << "Do you wish to continue? 1 for yes / 0 for no: ";
		cin >> didGiveUpOnTheGame;
	} while (!didGiveUpOnTheGame);
	

}

int main()
{
	string playerName;
	cout << "Enter your name:" << endl;
	cin >> playerName;
	char gameType;
	if (playerName == "Q")
		return 0;
	cout << "Type r for resuming your previous game, n for playing a new game" << endl;
	cin >> gameType;
	if (gameType != 'Q')
	{
		while ((gameType != 'r') || (gameType != 'c'))
		{
			cout << "Wrong input, please specify your orders again." << endl;
			cin >> gameType;
			if (gameType == 'Q')
			{
				didGiveUpOnTheGame = 1;
				return 0;
			}
		}
	}
	//currentScore = 0; 
    return 0;
}

