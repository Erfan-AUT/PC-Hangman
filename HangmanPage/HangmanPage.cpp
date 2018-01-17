#include "stdafx.h"
#include <cstdlib>
#include <cstdio>
#include <Windows.h>
#include <time.h>

static const int CHARMAX = 1000;
static const char* TOPICSLIST = "AVAILABLE_TOPICS.TXT";

bool didGiveUpOnTheGame = 0;
char* usedTopicAddress = "";

struct StringNode {
	char* string;
	StringNode* next;
};

StringNode *CreateStringNode(char line[]) {
	struct StringNode * nn;
	nn = (StringNode *)malloc(sizeof(StringNode));
	nn->string = strcpy(nn->string, line);
	nn->next = NULL;
	return nn;
}

void AddStringNodeBottom(char* val, StringNode *head) {

	StringNode *newStringNode = (StringNode*)malloc(sizeof(StringNode));
	newStringNode->string = val;
	newStringNode->next = NULL;
	if (head->next == NULL)
		head->next = newStringNode;
	else
	{
		StringNode *current = head;
		for (; current->next != NULL; current = current->next);
		current->next = newStringNode;
	}
}

StringNode* GenerateSubjectsSlashWords(char* fileName)
{
	StringNode* maind = (StringNode*)malloc(sizeof(StringNode));
	maind->next = NULL;
	maind->string = "";
	StringNode* currd = (StringNode*)malloc(sizeof(StringNode));
	FILE * file = fopen(fileName, "r");
	char line[1000];
	bool i = 0;
	while (fgets(line, CHARMAX, file) != NULL)
	{
		strtok(line, "\n");
		int t = strlen(line);
		if (!i)
		{
			char* q = (char*)malloc(sizeof(char*));
			strcpy(q, line);
			maind->string = q;
		}
		else
		{
			char* q = (char*)malloc(sizeof(char*));
			strcpy(q, line);
			AddStringNodeBottom(q, maind);
		}
		i++;
	}
	return maind;
}

bool SearchInsideTheStringNode(char* searchee, StringNode* searched)
{
	StringNode *current = searched;
	for (; current->next != NULL; current = current->next)
		if (!strcmp(current->string, searchee))
			return 1;
	if (!strcmp(current->string, searchee))
		return 1;
	return 0;
}

StringNode* RemoveElementFromStringNode(char* element, StringNode *current)
{
	if (current == NULL)
		return NULL;
	if (!strcmp(current->string, element))
	{
		StringNode *tempNextP;
		tempNextP = current->next;
		free(current);
		return tempNextP;
	}
	current->next = RemoveElementFromStringNode(element, current->next);
	return current;
}

StringNode* GenerateUnplayedSubjects(char* playerName, char gameType)
{
	StringNode *gonnaReturn;
	char* topicsListAddress = "AVAILABLE_TOPICS.txt";
	char line[CHARMAX], c;
	int showCounter = 1, totalScore, topicRecord;
	FILE* listOfTopics;
	listOfTopics = fopen(topicsListAddress, "r");
	if (gameType == 'n')
	{
		while (fscanf(listOfTopics, "%s", line) > 0)
		{
			printf("%d %s", showCounter, line);
			AddStringNodeBottom(line, gonnaReturn);
			showCounter++;
		}
	}
	if (gameType == 'r')
	{
		FILE* playerFile = fopen(playerName, "r");
		fscanf(playerFile, "%d", &totalScore);
		while (fscanf(playerFile, "%s %d", line, &topicRecord) > 0)
		{
			bool bl = SearchInsideTheStringNode(line, gonnaReturn);
			if (bl)
				gonnaReturn = RemoveElementFromStringNode(line, gonnaReturn);
			else
				printf("%d : %s", showCounter, line);
			showCounter++;
		}
	}
	return gonnaReturn;
}

int GetLinkedListSize(StringNode * head)
{
	int w = 1;
	for (StringNode * current = head; current->next != NULL; current = current->next)
		w++;
	return w;
}

char * ReturnNthLinkedListNumber(StringNode * head, int n)
{
	int w = 1;
	StringNode * current;
	for (current = head; current->next != NULL; current = current->next)
	{
		if (w == n)
			return current->string;
		w++;
	}
	return current->string;
}

void DrawingTheGuyPlusWrongGuesses(short numberOfWrongGuesses)
{

}

char* PlayOneSubjectUntilItsDone(char *playerName, char &gameType, StringNode* includedWords, int* totalScore1)
{
	int totalScore = *totalScore1;
	char* line;
	int randomWordNumber = rand() % GetLinkedListSize(includedWords);
	printf("\n Okay! Game will start in 3..2.. \n");
	system("cls");
	char* currentWord = ReturnNthLinkedListNumber(includedWords, randomWordNumber);
	includedWords = RemoveElementFromStringNode(currentWord, includedWords);
	printf("Now, the word consists of these letters, are you gonna be able to find them? \n ");
	for (int i = 0; i < strlen(currentWord); i++) printf("_ ");
	printf("\n \n Now, LET THE GAMES BEGIN! \n");
	short numberOfWrongGuesses = 0, numberofRightGuesses = 0;
	int currentScore = 0;
	char currentGuess;
	bool hasGuessedRight;
	char* whatToShow = "";
	while (numberOfWrongGuesses < 5)
	{
		hasGuessedRight = 0;
		printf("Enter your guess: ");
		scanf("%c", &currentGuess);
		if (currentGuess != 'Q')
		{
			for (int i = 0; i < strlen(currentWord); i++)
			{
				if (currentWord[i] == currentGuess)
				{
					whatToShow += currentWord[i] + ' ';
					hasGuessedRight = 1;
					numberofRightGuesses++;
				}
				else
					whatToShow += '_' + ' ';
			}
			printf("%s \n", whatToShow);
			if (!hasGuessedRight)
				numberOfWrongGuesses++;
			if (numberofRightGuesses == strlen(currentWord))
			{
				currentScore = 3 * strlen(currentWord) - numberOfWrongGuesses;
				printf("CONGRATS! YOU HAVE FIGURED THE WORD OUT! \n");
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
	return currentWord;
}

int returnPreviousTotalScore(char* playerName, char gameType)
{
	if (gameType == 'n')
		return 0;
	FILE* playerFile;
	playerFile = fopen(playerName, "r");
	int n;
	fscanf(playerFile, "%d", &n);
	return n;
}

double PlayCalcScoreOfOneSubject(char* playerName, char gameType)
{
	time_t starTimer = time(0);
	char* line, *selectedTopicAddress, *checkNotQ;
	printf("Please choose the number of one of the following subjects: \n");
	StringNode* listOfUnplayedSubjects = GenerateUnplayedSubjects(playerName, gameType);
	int whichTopicNumber;
	scanf("%s", &checkNotQ);
	if (checkNotQ != "Q")
	{
		whichTopicNumber = atoi(checkNotQ);
		selectedTopicAddress = ReturnNthLinkedListNumber(listOfUnplayedSubjects, whichTopicNumber);
		usedTopicAddress = selectedTopicAddress;
		free(listOfUnplayedSubjects);
		FILE * selectedTopicFile = fopen(selectedTopicAddress, "r");
		StringNode * includedWords;
		while (fscanf(selectedTopicFile, "%s", line) > 0)
			AddStringNodeBottom(line, includedWords);
		int totalScore = returnPreviousTotalScore(playerName, gameType);
		while (includedWords != NULL)
		{
			char* currentWord = PlayOneSubjectUntilItsDone(playerName, gameType, includedWords, &totalScore);
			includedWords = RemoveElementFromStringNode(currentWord, includedWords);
		}
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

void GeneralGamePlay(char* playerName, char gameType)
{
	playerName += '.';
	playerName += 't';
	playerName += 'x';
	playerName += 't';
	playerName += '.';
	do {
		double totalScore = PlayCalcScoreOfOneSubject(playerName, gameType);
		if (gameType == 'r')
		{
			FILE * playerFile = fopen(playerName, "a");
			usedTopicAddress += '\n';
			fprintf(playerFile, "%s", usedTopicAddress);
			usedTopicAddress -= '\n';
		}
		else
		{
			// Needs a full eval when it's specified.
		}
		printf("Here's your total score on this subject: %d \n Do you wish to continue? 1 for yes / 0 for no: ", totalScore);
		scanf("%d", didGiveUpOnTheGame);
	} while (!didGiveUpOnTheGame);
}

int main()
{
	char* playerName;
	scanf("Enter your name: %s \n", playerName);
	char gameType;
	if (playerName == "Q")
		return 0;
	scanf("Type r for resuming your previous game, n for playing a new game \n %c", gameType);
	if (gameType != 'Q')
	{
		while ((gameType != 'r') || (gameType != 'c'))
		{
			scanf("Wrong input, please specify your orders again. \n %c", gameType);
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