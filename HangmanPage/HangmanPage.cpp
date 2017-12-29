// HangmanPage.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

ifstream& GotoLine(ifstream &file, unsigned int num) 
{
	file.seekg(std::ios::beg);
	for (int i = 0; i < num - 1; ++i) {
		file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	return file;
}

int main()
{
	string playerName, line, topicsListAddress = "AVAILABLE_TOPICS.txt", selectedTopicAddress;
	cout << "Enter your name:" << endl;
	cin >> playerName;
	char gameType; int showcounter = 1, realcounter = 0, whichgamenumber;
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
		cout << showcounter << ":  " << line << endl;
		showcounter++;
	}
	cin >> whichgamenumber;
	GotoLine(listOfTopics, showcounter);
	listOfTopics >> selectedTopicAddress;
	ifstream selectedTopicFile(selectedTopicAddress);
	vector <string> includedWords;
	while (!selectedTopicFile.eof())
	{
		getline(selectedTopicFile, line);
		includedWords.push_back(line);
	}
    return 0;
}

