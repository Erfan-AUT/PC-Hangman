#include "stdafx.h"
#include <cstdlib>
#include <cstdio>
#include <Windows.h>
#include <time.h>

static const int CHARMAX = 1000;
static char* TOPICSLIST = "AVAILABLE_TOPICS.TXT";
static char* PLAYERSLIST = "PLAYERS.TXT";

bool didGiveUpOnTheGame = 0;

int previousWrongGuess = 0;

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

bool SearchInsideString(char* searchee, char searched)
{
	for (int i = 0; i < strlen(searchee); i++)
		if (searched == searchee[i])
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
		//free(current);
		return tempNextP;
	}
	current->next = RemoveElementFromStringNode(element, current->next);
	return current;
}

SubjectNode* RemoveElementFromSubjectNode(char* element, SubjectNode *current)
{
	if (current == NULL)
		return NULL;
	if (!strcmp(current->nameOfSubject, element))
	{
		SubjectNode *tempNextP;
		tempNextP = current->next;
		return tempNextP;
	}
	current->next = RemoveElementFromSubjectNode(element, current->next);
	return current;
}

StringNode* GenerateUnplayedSubjects(char* playerName, char gameType)
{
	StringNode *gonnaReturn = (StringNode*)malloc(sizeof(StringNode*));
	char* topicsListAddress = TOPICSLIST;
	char line[CHARMAX];
	int showCounter = 1, totalScore;
	double topicRecord;
	FILE* listOfTopics = fopen(topicsListAddress, "r");
	fscanf(listOfTopics, "%s", line);
	char* q = (char*)malloc(sizeof(char*));
	strcpy(q, line);
	gonnaReturn->string = q;
	gonnaReturn->next = NULL;
	while (fscanf(listOfTopics, "%s", line) > 0)
	{
		char* q = (char*)malloc(sizeof(char*));
		strcpy(q, line);
		AddStringNodeBottom(q, gonnaReturn);
	}
	if (gameType == 'r')
	{
		FILE* playerFile = fopen(playerName, "r");
		//fscanf(playerFile, "%d", &totalScore);
		while (fscanf(playerFile, "%s %lf", line, &topicRecord) > 0)
		{
			if (SearchInsideTheStringNode(line, gonnaReturn))
				gonnaReturn = RemoveElementFromStringNode(line, gonnaReturn);
			else
			showCounter++;
		}
	}
	StringNode *gonnaPrint = gonnaReturn;
	for (; gonnaPrint != NULL; gonnaPrint = gonnaPrint->next)
	{
		printf("%d: %s \n", showCounter, gonnaPrint->string);
		showCounter++;
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
	for (current = head; current!= NULL; current = current->next)
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
	for (current = head; current != NULL; current = current->next)
	{
		if (w == n)
			return current;
		w++;
	}
	return current;
}

void DrawingTheGuy(short numberOfWrongGuesses)
{
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 8);
	for (int i = 4; i < 26; i++)
	{
		gotoxy(58, i);
		for (int j = 58; j < 95; j++)
			printf("%c", ' ');
	}
	int i = 70, j = 4, lastX = 95, lastY = 25, firstX = 58, firstY;
	{
		gotoxy(i, j);
		//Hairline is strong, bro.
		gotoxy(i, j - 1);
		for (int q = 0; q < 12; q++)
			printf("%c", char(244));
		gotoxy(i, j);
		// Draw the head
		SetConsoleTextAttribute(hConsole, 14);
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
		for (int q = 9; q < 14; q++)
		{
			gotoxy(i + 11, q - j);
			printf("%c", char(186));
		}
		//Draw the face
		SetConsoleTextAttribute(hConsole, 9);
		gotoxy(i + 2, j + 1);
		printf("%c", char(233));
		gotoxy(i + 8, j + 1);
		printf("%c", char(233));
		SetConsoleTextAttribute(hConsole, 14);
		gotoxy(i + 5, j + 2);
		printf("%c", char(186));
		SetConsoleTextAttribute(hConsole, 8);
		gotoxy(i + 2, j + 4);
		for (int t = 0; t < 7; t++)
			printf("%c", char(176));
		i += 6;
		j += 7;
		SetConsoleTextAttribute(hConsole, 14);
		gotoxy(i - 3, j);
		printf("%c", char(186));
		gotoxy(i + 2, j);
		printf("%c", char(186));
		// Neck.
		SetConsoleTextAttribute(hConsole, 13);
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
		SetConsoleTextAttribute(hConsole, 14);
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
		SetConsoleTextAttribute(hConsole, 10);
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
		gotoxy(i + 4, j + 12);
		printf("%c", char(186));
		//Legs
		SetConsoleTextAttribute(hConsole, 12);
		gotoxy(i - 3, j + 13);
		printf("%c", char(185));
		gotoxy(i - 6, j + 14);
		printf("%c%c%c%c", char(200), char(205), char(205), char(188));
		gotoxy(i + 4, j + 13);
		printf("%c", char(204));
		gotoxy(i + 4, j + 14);
		printf("%c%c%c%c", char(200), char(205), char(205), char(188));
		gotoxy(i - 6, j + 13);
		printf("%c%c%c", char(201), char(205), char(205));
		gotoxy(i + 5, j + 13);
		printf("%c%c%c", char(205), char(205), char(187));
		//Feet
	}
	if (numberOfWrongGuesses == 5)
		firstY = 2;
	gotoxy(70, 2);
	if (numberOfWrongGuesses == 4)
		firstY = 9;
	gotoxy(70, 8);
	if (numberOfWrongGuesses == 3)
		firstY = 11;
	if (numberOfWrongGuesses == 2)
		firstY = 19;
	if (numberOfWrongGuesses == 1)
		firstY = 23;
	if (numberOfWrongGuesses == 0)
		firstY = 25;
	int sleepCounter;
	SetConsoleTextAttribute(hConsole, 155);
	for (; firstY != lastY; lastY--)
	{
		gotoxy(firstX, lastY);
		for (int i = lastX; i >= firstX; i--)
		{
			printf("%c", char(247));
			if ((((numberOfWrongGuesses == 5) && (lastY <= 9)) ||
				((numberOfWrongGuesses == 4) && (lastY <= 11)) ||
				((numberOfWrongGuesses == 3) && (lastY <= 19)) ||
				((numberOfWrongGuesses == 2) && (lastY <= 23)) ||
				((numberOfWrongGuesses == 1) && (lastY <= 25)) ||
				(numberOfWrongGuesses == 0)) && (numberOfWrongGuesses != previousWrongGuess))
				_sleep(5);
		}
		if ((((numberOfWrongGuesses == 5) && (lastY <= 9)) ||
			((numberOfWrongGuesses == 4) && (lastY <= 11)) ||
			((numberOfWrongGuesses == 3) && (lastY <= 19)) ||
			((numberOfWrongGuesses == 2) && (lastY <= 23)) ||
			((numberOfWrongGuesses == 1) && (lastY <= 25)) ||
			(numberOfWrongGuesses == 0)) && (numberOfWrongGuesses != previousWrongGuess))
			_sleep(5);
	}
	previousWrongGuess = numberOfWrongGuesses;
	SetConsoleTextAttribute(hConsole, 15);
}

char* PlayOneSubjectUntilItsDone(char *playerName, char gameType, StringNode* includedWords, double* totalScore)
{
	srand(time(0));
	char* line;
	char wrongGuesses[6] = { ' ', ' ', ' ', ' ', ' ', ' ' };
	int randomWordNumber = rand() % GetLinkedListSize(includedWords) + 1;
	printf("\n Okay! Game will start in 3..2.. \n");
	system("cls");
	char* currentWord = ReturnNthLinkedListNumber(includedWords, randomWordNumber);
	char* showBiz = (char*)calloc(2 * strlen(currentWord), sizeof(char));
	char* checkBiz = (char*)malloc(sizeof(char*));
	strcpy(checkBiz, currentWord);
	for (int i = 0; i < strlen(currentWord); i++)
		checkBiz[i] = char(187);
	for (int i = 0; i < strlen(currentWord); i++)
		strcat(showBiz, "_ ");
	printf("\n \n Now, LET THE GAME BEGIN! \n");
	_sleep(500);
	short numberOfWrongGuesses = 0;
	int currentScore = 0;
	char currentGuess = ' ';
	bool hasGuessedRight;
	//char* whatToShow = "";
	while (numberOfWrongGuesses < 5)
	{
		system("cls");
		hasGuessedRight = 0;
		printf("Now, the word consists of these letters, are you gonna be able to find them? \n%s", showBiz);
		// Show current Status.
		DrawingTheGuy(numberOfWrongGuesses);
		gotoxy(0, 8);
		printf("Your guesses thus far has been: \n");
		for (int i = 1; i <= numberOfWrongGuesses; i++)
			printf("%c ", wrongGuesses[i]);
		gotoxy(0, 3);
		printf("Enter your guess: ");
		scanf(" %c", &currentGuess);
		if (currentGuess != 'Q')
		{
			gotoxy(0, 1);
			for (int i = 0; i < strlen(currentWord); i++)
			{
				if (currentWord[i] == currentGuess)
				{
					showBiz[2 * i] = currentGuess;
					hasGuessedRight = 1;
					if (i == strlen(currentWord) - 1)
						hasGuessedRight = 1;
					checkBiz[i] = currentGuess;
				}
			}
			bool isDouble = SearchInsideString(wrongGuesses, currentGuess);
			if ((!hasGuessedRight) && (!isDouble))
				numberOfWrongGuesses++;
			if (!strcmp(checkBiz, currentWord))
			{
				currentScore = 3 * strlen(currentWord) - numberOfWrongGuesses;
				gotoxy(0, 5);
				printf("CONGRATS! YOU HAVE FIGURED THE WORD OUT! \n");
				printf("The word was: %s", currentWord);
				_sleep(1000);
				break;
			}
			else if ((!isDouble) && (!SearchInsideString(checkBiz, currentGuess)))
				wrongGuesses[numberOfWrongGuesses] = currentGuess;
		}
		else
		{
			didGiveUpOnTheGame = 1;
			return 0;
		}
	}
	if (numberOfWrongGuesses == 5)
	{
		printf("YOU HAVE FAILED THIS CITY \n");
		printf("The word was: %s", currentWord);
		_sleep(1000);
	}
	DrawingTheGuy(numberOfWrongGuesses);
	*totalScore += currentScore;
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
	fclose(playerFile);
	return n;
}

void SavingShit(char* playerName, SubjectNode* stars)
{
	//Should test "a" with non-existent files. 
	//Works just fine!
	int n = 1;
	FILE* playerFile = fopen(playerName, "a");
	SubjectNode *current = (SubjectNode*)malloc(sizeof(SubjectNode*));
	while (current != NULL)
	{
		current = ReturnNthSubjectList(stars, n);
		if (current != NULL)
			fprintf(playerFile, "%s   %lf\n", current->nameOfSubject, current->scoreOfSubject);
		n++;
	}
}

void SaveInPlayers(char* playerName, char gameType)
{
	if (gameType != 'r')
	{
		FILE* playersList = fopen(PLAYERSLIST, "a");
		fprintf(playersList, "%s\n", playerName);
		fclose(playersList);
	}
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
		double totalScore = 0;
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

void ClearShit(char* playerName, SubjectNode* stars)
{
	FILE* playerFile = fopen(playerName, "r");
	int w = 1;
	char line[CHARMAX];
	double topicRecord;
	SubjectNode * head = (SubjectNode*)malloc(sizeof(SubjectNode));
	fscanf(playerFile, "%s %lf", line, &topicRecord);
	head->nameOfSubject = (char*)malloc(sizeof(char*));
	strcpy(head->nameOfSubject, line);
	head->scoreOfSubject = topicRecord;
	head->next = NULL;
	while (fscanf(playerFile, "%s %lf", line, &topicRecord) > 0)
	{
		char* q = (char*)malloc(sizeof(char*));
		strcpy(q, line);
		AddSubjectNodeBottom(q, topicRecord, head);
	}
	SubjectNode * current = (SubjectNode*)malloc(sizeof(SubjectNode));
	for (current = head; current->next != NULL; current = current->next)
		if (!strcmp(current->nameOfSubject,current->next->nameOfSubject))
			head = RemoveElementFromSubjectNode(current->nameOfSubject, head);
}

void GeneralGamePlay(char* playerName, char gameType)
{
	int gonnaSave = 1, numOfGames = 1;
	SubjectNode *allSubjects = (SubjectNode*)malloc(sizeof(SubjectNode*));
	char* line;
	strcat(playerName, ".txt");
	do {
		if (numOfGames == 1)
		{
			allSubjects = PlayCalcScoreOfOneSubject(playerName, gameType);
			allSubjects->next = NULL;
		}
		else
		{
			SubjectNode *temp = (SubjectNode*)malloc(sizeof(SubjectNode*));
			temp = PlayCalcScoreOfOneSubject(playerName, gameType);
			AddSubjectNodeBottom(temp->nameOfSubject, temp->scoreOfSubject, allSubjects);
		}
		if (!didGiveUpOnTheGame)
		{
			system("cls");
			SubjectNode *temp = ReturnNthSubjectList(allSubjects, numOfGames);
			printf("Here's your total score on this subject: %lf \n Do you wish to continue? 0 for yes | 1 for no: ",
				ReturnNthSubjectList(allSubjects, numOfGames)->scoreOfSubject);
			scanf(" %d", &didGiveUpOnTheGame);
			if (!didGiveUpOnTheGame)
				SavingShit(playerName, allSubjects);
			numOfGames++;
		}
	} while (!didGiveUpOnTheGame);
	remove(playerName);
	printf("Do you wish to save the game? 1 for yes | 0 for no: ");
	scanf(" %d", &gonnaSave);
	if (gonnaSave)
	{
		SavingShit(playerName, allSubjects);
		ClearShit(playerName, allSubjects);
		SaveInPlayers(playerName, gameType);
	}
	return;
}

void TopicGenerator()
{
	char* a = (char*)malloc(sizeof(char*));
	printf("Ok, give me the name of your to-be-created topic: ");
	scanf(" %s", a);
	FILE* mainList = fopen(TOPICSLIST, "a");
	strcat(a, ".txt");
	FILE *newList = fopen(a, "w");
	fprintf(mainList, "\n%s", a);
	printf("\n Now, add your chosen words and at the end of them, enter 0. \n");
	while (scanf("%s", a) > 0)
	{
		if (!strcmp(a, "0"))
			break;
		fprintf(newList, "%s  ", a);
	}
}

void ShowTopTen()
{
	int howManyPlayers = 1;
	char* line = (char*)malloc(sizeof(char*));
	double dees;
	FILE * listOfPlayers = fopen(PLAYERSLIST, "r");
	StringNode *gonnaReturn = (StringNode*)malloc(sizeof(StringNode*));
	SubjectNode* allPlayersSumDouble = (SubjectNode*)malloc(sizeof(SubjectNode*));
	fscanf(listOfPlayers, "%s", line);
	char* q = (char*)malloc(sizeof(char*));
	strcpy(q, line);
	gonnaReturn->string = q;
	gonnaReturn->next = NULL;
	while (fscanf(listOfPlayers, "%s", line) > 0)
	{
		char* q = (char*)malloc(sizeof(char*));
		strcpy(q, line);
		AddStringNodeBottom(q, gonnaReturn);
		howManyPlayers++;
	}
	SubjectNode *head = (SubjectNode*)malloc(sizeof(SubjectNode*));
	char* name = ReturnNthLinkedListNumber(gonnaReturn, 1);
	FILE * tempPlayer = fopen(name, "r");
	double sum = 0;
	while (fscanf(tempPlayer, "%s %lf", line, &dees) > 0)
		sum += dees;
	head->nameOfSubject = (char*)malloc(sizeof(char*));
	strcpy(head->nameOfSubject, name);
	head->scoreOfSubject = sum;
	head->next = NULL;
	for (int i = 2; i <= howManyPlayers; i++)
	{
		name = ReturnNthLinkedListNumber(gonnaReturn, i);
		FILE * tempPlayer = fopen(name, "r");
		sum = 0;
		while (fscanf(tempPlayer, "%s %lf", line, &dees) > 0)
			sum += dees;
		AddSubjectNodeBottom(name, sum, head);
	}
	int nPla = howManyPlayers;
	char* goat = (char*)malloc(sizeof(char*));
	double maxScore = 0;
	while (nPla > 1)
	{
		SubjectNode* kk = ReturnNthSubjectList(head, 1);
		maxScore = 0;
		goat = (char*)malloc(sizeof(char*));
		while (kk != NULL)
		{
			double tempScore = kk->scoreOfSubject;
			if (tempScore > maxScore)
			{
				maxScore = tempScore;
				strcpy(goat, kk->nameOfSubject);
			}
			kk = kk->next;
		}
		head = RemoveElementFromSubjectNode(goat, head);
		printf("%d. %s with the total score of: %lf \n", howManyPlayers - nPla + 1, goat, maxScore);
		nPla--;
	}
	if (howManyPlayers != 10)
	{
		SubjectNode* kk = ReturnNthSubjectList(head, 1);
		printf("%d. %s with the total score of: %lf \n", howManyPlayers - nPla + 1, kk->nameOfSubject, kk->scoreOfSubject);
	}
	_sleep(10000);
}

int main()
{
	char* playerName = (char*) malloc (sizeof(char*));
	printf("Enter your name: ");
	scanf("%s", playerName);
	char* gameType1 = (char*)malloc(sizeof(char*));
	if (playerName == "Q")
		return 0;
	printf("Type r for resuming your previous game, n for playing a new game, t for topic generator, o for showing top ten: ");
	scanf(" %s", gameType1);
	char gameType = gameType1[0];
	if (gameType != 'Q')
	{
		while ((gameType != 'r') && (gameType != 'n') && (gameType != 't') && (gameType != 'o'))
		{
			printf("Wrong input, please specify your orders again. \n");
			scanf(" %c", &gameType);
			if (gameType == 'Q')
			{
				didGiveUpOnTheGame = 1;
				return 0;
			}
		}
		if ((gameType == 'r') || (gameType == 'n'))
			GeneralGamePlay(playerName, gameType);
		else if (gameType == 't')
			TopicGenerator();
		else
			ShowTopTen();
	}
	//currentScore = 0; 
	return 0;
}