#include <iostream>
#include <cstring>
#include <Windows.h>

#include <locale.h>

using namespace std;

#define NUMBER 1024
#define LENGTH_SHINGLE 3

const string separator = "./,+-#$%^&*()=!?“”— ";
const string numbers = "0123456789";

char words[NUMBER][NUMBER] = {0};
char textWords[NUMBER][NUMBER] = {0};
char fragmentWords[NUMBER][NUMBER] = {0};

int getNumberPartitionedWords(string str, char array[][NUMBER]);
int strCmp(char firstString[], char secondString[]);

double antiPlagiarism(string text, string fragment);
double compareShingles(char textWords[][NUMBER], char fragmentWords[][NUMBER], int textCounter, int fragmentCounter);

bool isSeparator(char symbol);

bool isNumber(char symbol);
void showWords(char wordsArr[][NUMBER], int wordsCount);

int main()
{
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    // system("chcp 1251");

    string text = "Всем привет, как ваши дела, и как вы себя чувствуете?) Чем вы занимайтесь? Давайте сходим куда-нить погулять.";
    string fragment = "Всем привет, как ваши дела, и как вы себя чувствуете!";

    cout << "Percent of anti plagiarism in text " << antiPlagiarism(text, fragment) << "%" << endl;

    return 0;
}

double antiPlagiarism(string text, string fragment)
{
    double textCounter = getNumberPartitionedWords(text, textWords);
    double fragmentCounter = getNumberPartitionedWords(fragment, fragmentWords);

    showWords(textWords, textCounter);
    cout << endl;
    showWords(fragmentWords, fragmentCounter);

    double shingleCounter = compareShingles(textWords, fragmentWords, textCounter, fragmentCounter);
    cout << shingleCounter << endl;

    return shingleCounter * 100.0 / (fragmentCounter - LENGTH_SHINGLE + 1);
}

int getNumberPartitionedWords(string str, char array[][NUMBER])
{
    int wordsCounter = 0, wordsPointer = 0;

    for (int i = 0; str[i] != '\0'; i++)
    {
        if (!isSeparator(str[i]) && !isSeparator(str[i + 1]))
        {
            array[wordsCounter][wordsPointer++] = str[i];
        }

        if (!isSeparator(str[i]) && (isSeparator(str[i + 1])))
        {
            array[wordsCounter++][wordsPointer] = str[i];
            wordsPointer = 0;
        }

        if (!isSeparator(str[i]) && str[i + 1] == '\0')
        {
            array[++wordsCounter][wordsPointer] = str[i];
            wordsPointer = 0;
        }
    }

    return wordsCounter;
}

int strCmp(char firstString[], char secondString[])
{
    for (int i = 0; firstString[i] != '\0'; i++)
    {
        if (firstString[i] > secondString[i])
        {
            return 1;
        }

        if (firstString[i] < secondString[i])
        {
            return -1;
        }
    }

    return 0;
}

double compareShingles(char textWords[][NUMBER], char fragmentWords[][NUMBER], int textCounter, int fragmentCounter)
{
    int shingleCountText = (textCounter - LENGTH_SHINGLE) + 1;
    int shingleCountFragment = (fragmentCounter - LENGTH_SHINGLE) + 1;

    cout << shingleCountText << endl;
    cout << shingleCountFragment << endl;
    double counter = 0, gg = 0;

    for (int i = 0; i < shingleCountFragment; i++) // shingles of fragment
    {
        for (int j = 0; j < shingleCountText; j++) // shingles of text
        {
            bool isSame = true;

            for (int n = 0; n < LENGTH_SHINGLE; n++) // every word in shingle
            {
                if (strCmp(fragmentWords[i + n], textWords[j + n])) // compare every letter in word
                {
                    isSame = false;
                    break;
                }
            }

            if (!isSame)
            {
                continue;
            }

            counter++;
        }
    }

    return counter;
}

void showWords(char wordsArr[][NUMBER], int wordsCount)
{
    for (int i = 0; i < wordsCount; i++)
    {
        for (int j = 0; wordsArr[i][j] != '\0'; j++)
        {
            cout << wordsArr[i][j];
        }
        cout << endl;
    }
}

bool isSeparator(char symbol)
{
    for (int i = 0; separator[i] != '\0'; i++)
    {
        if (symbol == separator[i])
        {
            return true;
        }
    }

    return false;
}

bool isNumber(char symbol)
{
    for (int i = 0; numbers[i] != '\0'; i++)
    {
        if (symbol == numbers[i])
        {
            return false;
        }
    }

    return true;
}
