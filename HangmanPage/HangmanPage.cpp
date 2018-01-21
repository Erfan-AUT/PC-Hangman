#include "stdafx.h"
#include <cstdlib>
#include <cstdio>
#include <Windows.h>
#include <time.h>

static const int CHARMAX = 1000;
static char* TOPICSLIST = "AVAILABLE_TOPICS.TXT";

bool didGiveUpOnTheGame = 0;
char* usedTopicAddress = "";

void gotoxy(int x, int y)
{
	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x, y };
	SetConsoleCursorPosition(h, c);
}

struct StringNode {
	char* string;
	StringNode* next;
};

struct SubjectNode
{
	char* nameOfSubject;
	double scoreOfSubject;
	SubjectNode* next;
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

void AddSubjectNodeBottom(char* name, double score, SubjectNode *head) {

	SubjectNode *newSubjectNode = (SubjectNode*)malloc(sizeof(SubjectNode));
	newSubjectNode->nameOfSubject = name;
	newSubjectNode->scoreOfSubject = score;
	newSubjectNode->next = NULL;
	if (head->next == NULL)
		head->next = newSubjectNode;
	else
	{
		SubjectNode *current = head;
		for (; current->next != NULL; current = current->next);
		current->next = newSubjectNode;
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
	StringNode *gonnaReturn = (StringNode*)malloc(sizeof(StringNode*));
	char* topicsListAddress = TOPICSLIST;
	char line[CHARMAX], c;
	int showCounter = 1, totalScore, topicRecord;
	FILE* listOfTopics;
	listOfTopics = fopen(topicsListAddress, "r");
	if (gameType == 'n')
	{
		fscanf(listOfTopics, "%s", line);
		char* q = (char*)malloc(sizeof(char*));
		strcpy(q, line);
		gonnaReturn->string = q;
		gonnaReturn->next = NULL;
		printf("%d %s \n", showCounter, line);
		while (fscanf(listOfTopics, "%s", line) > 0)
		{
			printf("%d %s \n", showCounter, line);
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

SubjectNode* ReturnNthSubjectList(SubjectNode * head, int n)
{
	int w = 1;
	SubjectNode * current;
	for (current = head; current->next != NULL; current = current->next)
	{
		if (w == n)
			return current;
		w++;
	}
	return current;
}

void DrawingTheGuy(short numberOfWrongGuesses)
{
	int i = 70, j = 2;
	gotoxy(i, j);
	if (numberOfWrongGuesses < 5)
	{
		//Hairline is strong, bro.
		gotoxy(i, j - 1);
		for (int q = 0; q < 10; q++)
			printf("%c", char(244));
	}
	if (numberOfWrongGuesses < 4) 
	{
		gotoxy(i, j);
		// Draw the head
		printf("%c", char(201));
		for (int q = 0; q < 10; q++)
			printf("%c", char(205));
		printf("%c", char(187));
		for (int q = 1; q < 7; q++)
		{
			gotoxy(i, j + q);
			printf("%c", char(186));
		}
		gotoxy(i, j + 6);
		printf("%c", char(200));
		for (int q = 0; q < 10; q++)
		{
			if ((q == 2) || (q == 7))
				printf("%c", char(203));
			else
				printf("%c", char(205));
		}
		printf("%c", char(188));
		for (int q = 5; q < 10; q++)
		{
			gotoxy(i + 11, q - j);
			printf("%c", char(186));
		}
		//Draw the face
		gotoxy(i + 2, j + 1);
		printf("%c", char(233));
		gotoxy(i + 8, j + 1);
		printf("%c", char(233));
		gotoxy(i + 5, j + 2);
		printf("%c", char(186));
		gotoxy(i + 2, j + 4);
		for (int t = 0; t < 7; t++)
			printf("%c", char(176));
		i += 6;
		j += 7;
	}
	if (numberOfWrongGuesses < 3)
	{
		gotoxy(i - 3, j);
		printf("%c", char(186));
		gotoxy(i + 2, j);
		printf("%c", char(186));
		// Neck.
		for (int t = -11; t < 12; t++)
		{
			gotoxy(i + t, j + 1);
			if ((t == -3) || (t == 2))
				printf("%c", char(202));
			else
				printf("%c", char(205));
		}
		printf("%c", char(187));
		gotoxy(i - 11, j + 1);
		printf("%c", char(201));
		for (int q = 2; q < 10; q++)
		{
			gotoxy(i - 11, j + q);
			if ((q == 3) || (q == 5))
				printf("%c", char(185));
			else
				printf("%c", char(186));
		}
		gotoxy(i - 11, j + 9);
		printf("%c", char(200));
		for (int q = 0; q < 22; q++)
		{
			if ((q == 7) || (q == 14))
				printf("%c", char(203));
			else
				printf("%c", char(205));
		}
		printf("%c", char(188));
		for (int q = -8; q < -1; q++)
		{
			gotoxy(i + 12, j - q);
			if ((q == -3) || (q == -5))
				printf("%c", char(204));
			else
				printf("%c", char(186));
		}
		// Main body.
		for (int q = 0; q < 6; q++)
		{
			gotoxy(i + 13 + q, j + 3);
			printf("%c", char(205));
			gotoxy(i + 13 + q, j + 5);
			printf("%c", char(205));
		}
		for (int q = 0; q < 6; q++)
		{
			gotoxy(i - 17 + q, j + 3);
			printf("%c", char(205));
			gotoxy(i - 17 + q, j + 5);
			printf("%c", char(205));
		}
		//Hands.
		//Change i here.
	}
	if (numberOfWrongGuesses < 2)
	{
		gotoxy(i - 3, j + 10);
		printf("%c", char(186));
		gotoxy(i + 4, j + 10);
		printf("%c", char(186));
		gotoxy(i - 3, j + 11);
		printf("%c", char(186));
		gotoxy(i + 4, j + 11);
		printf("%c", char(186));
		gotoxy(i - 3, j + 12);
		printf("%c", char(186));
		gotoxy(i - 3, j + 13);
		printf("%c", char(188));
		gotoxy(i + 4, j + 12);
		printf("%c", char(186));
		gotoxy(i + 4, j + 13);
		printf("%c", char(200));
		//Legs
	}
	if (numberOfWrongGuesses < 1)
	{
		//Feet.
	}
}

char* PlayOneSubjectUntilItsDone(char *playerName, char &gameType, StringNode* includedWords, double* totalScore1)
{
	double totalScore = *totalScore1;
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
	char currentGuess = ' ';
	bool hasGuessedRight;
	//char* whatToShow = "";
	while (numberOfWrongGuesses < 5)
	{
		system("cls");
		hasGuessedRight = 0;
		// Show current Status.
		DrawingTheGuy(numberOfWrongGuesses);
		gotoxy(0, 1);
		printf("\n Enter your guess: ");
		scanf(" %c", &currentGuess);
		if (currentGuess != 'Q')
		{
			for (int i = 0; i < strlen(currentWord); i++)
			{
				if (currentWord[i] == currentGuess)
				{
					printf("%c ", currentGuess);
					hasGuessedRight = 1;
					numberofRightGuesses++;
				}
				else
					printf("_ ");
			}
			if (!hasGuessedRight)
				numberOfWrongGuesses++;
			if (numberofRightGuesses == strlen(currentWord))
			{
				currentScore = 3 * strlen(currentWord) - numberOfWrongGuesses;
				printf("CONGRATS! YOU HAVE FIGURED THE WORD OUT! \n");
				break;
			}
			DrawingTheGuy(numberOfWrongGuesses);
		}
		else
		{
			didGiveUpOnTheGame = 1;
			return 0;
		}
	}
	totalScore += currentScore;
	return currentWord;
}

double returnPreviousTotalScore(char* playerName, char gameType)
{
	if (gameType == 'n')
		return 0;
	FILE* playerFile;
	playerFile = fopen(playerName, "r");
	double n;
	fscanf(playerFile, "%lf", &n);
	return n;
}

SubjectNode* PlayCalcScoreOfOneSubject(char* playerName, char gameType)
{
	SubjectNode* whatToReturn = (SubjectNode*)malloc(sizeof(SubjectNode*));
	time_t starTimer = time(0);
	char *selectedTopicAddress, *checkNotQ = (char*) malloc (sizeof(char*));
	char line[CHARMAX];
	printf("Please choose the number of one of the following subjects: \n");
	StringNode* listOfUnplayedSubjects = GenerateUnplayedSubjects(playerName, gameType);
	int whichTopicNumber;
	scanf("%s", checkNotQ);
	if (checkNotQ != "Q")
	{
		whichTopicNumber = atoi(checkNotQ);
		selectedTopicAddress = ReturnNthLinkedListNumber(listOfUnplayedSubjects, whichTopicNumber);
		usedTopicAddress = selectedTopicAddress;
		//free(listOfUnplayedSubjects);
		FILE * selectedTopicFile = fopen(selectedTopicAddress, "r");
		StringNode * includedWords = (StringNode*)malloc(sizeof(StringNode));
		fscanf(selectedTopicFile, "%s", line);
		includedWords->string = (char*)malloc(sizeof(char*));
		strcpy(includedWords->string, line);
		includedWords->next = NULL;
		while (fscanf(selectedTopicFile, "%s", line) > 0)
		{
			char* q = (char*)malloc(sizeof(char*));
			strcpy(q, line);
			AddStringNodeBottom(q, includedWords);
		}
		double totalScore = returnPreviousTotalScore(playerName, gameType);
		while (includedWords != NULL)
		{
			char* currentWord = PlayOneSubjectUntilItsDone(playerName, gameType, includedWords, &totalScore);
			if (currentWord == NULL)
			{
				time_t endTimer = time(0);
				double finalScore = (double)(totalScore / difftime(endTimer, starTimer));
				whatToReturn->nameOfSubject = selectedTopicAddress;
				whatToReturn->scoreOfSubject = finalScore;
				return whatToReturn;
			}
			includedWords = RemoveElementFromStringNode(currentWord, includedWords);
		}
		//ReadNthLine(selectedTopicAddress, listOfTopics, whichTopicNumber);
		time_t endTimer = time(0);
		double finalScore = (double)(totalScore / difftime(endTimer, starTimer));
		whatToReturn->nameOfSubject = selectedTopicAddress;
		whatToReturn->scoreOfSubject = finalScore;
		return whatToReturn;
	}
	else
	{
		didGiveUpOnTheGame = 1;
		return NULL;
	}
}

void SavingShit(char* playerName, SubjectNode* stars)
{
	//Should test "a" with non-existent files. 
	int n = 1;
	FILE* playerFile = fopen(playerName, "a");
	SubjectNode *current = stars;
	while (current != NULL)
	{
		current = ReturnNthSubjectList(current, n);
		fprintf(playerFile, "%s   %lf\n", current->nameOfSubject, current->scoreOfSubject);
		n++;
	}
}

void GeneralGamePlay(char* playerName, char gameType)
{
	bool gonnaSave, numOfGames = 0;
	SubjectNode *allSubjects = (SubjectNode*)malloc(sizeof(SubjectNode*));
	double totalScore;
	strcat(playerName, ".txt");
	do {
		FILE* playerFile;
		if (gameType == 'r')
		{
			playerFile = fopen(playerName, "a");
			fprintf(playerFile, "%s %s", usedTopicAddress, "  ");
			fprintf(playerFile, "%lf %c", totalScore, '\n');
		}
		else
			playerFile = fopen(playerName, "w");
		if (!numOfGames)
			allSubjects = PlayCalcScoreOfOneSubject(playerName, gameType);
		else
		{
			SubjectNode *temp = (SubjectNode*)malloc(sizeof(SubjectNode*));
			temp = PlayCalcScoreOfOneSubject(playerName, gameType);
			AddSubjectNodeBottom(temp->nameOfSubject, temp->scoreOfSubject, allSubjects);
		}
		if (!didGiveUpOnTheGame)
		{
			printf("Here's your total score on this subject: %d \n Do you wish to continue? 1 for yes | 0 for no: ", totalScore);
			scanf("%d", didGiveUpOnTheGame);
			numOfGames++;
		}
	} while (!didGiveUpOnTheGame);
	printf("Do you wish to save the game? 1 for yes | 0 for no");
	scanf("%d", gonnaSave);
	if (gonnaSave)
		SavingShit(playerName, allSubjects);
}

int main()
{
	char* playerName = (char*) malloc (sizeof(char*));
	printf("Enter your name: ");
	scanf("%s", playerName);
	char* gameType1 = (char*)malloc(sizeof(char*));
	if (playerName == "Q")
		return 0;
	printf("Type r for resuming your previous game, n for playing a new game: ");
	scanf(" %s", gameType1);
	char gameType = gameType1[0];
	if (gameType != 'Q')
	{
		while ((gameType != 'r') && (gameType != 'n'))
		{
			scanf("Wrong input, please specify your orders again. \n %c", gameType);
			if (gameType == 'Q')
			{
				didGiveUpOnTheGame = 1;
				return 0;
			}
		}
		GeneralGamePlay(playerName, gameType);
	}
	//currentScore = 0; 
	return 0;
}