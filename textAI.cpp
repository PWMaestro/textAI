#include <iostream>
#include <cstring>
#include <Windows.h>

using namespace std;

#define NUMBER 2500
#define LENGTH_SHINGLE 3

const string separator = {'.', ',', '/', '+', '-', '—', '#', '$', '%', '^', '&', '*', '(', ')', '=', '!', '?', '“', '”', ' '};
const string numbers = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
const string lowerCaseLetters = {'а', 'б', 'в', 'г', 'д', 'е', 'ё', 'ж', 'з', 'и', 'й', 'к', 'л', 'м', 'н', 'о', 'п', 'р', 'с', 'т', 'у', 'ф', 'х', 'ц', 'ч', 'ш', 'щ', 'э', 'ю', 'я'};
const string upperCaseLetters = {'А', 'Б', 'В', 'Г', 'Д', 'Е', 'Ё', 'Ж', 'З', 'И', 'Й', 'К', 'Л', 'М', 'Н', 'О', 'П', 'Р', 'С', 'Т', 'У', 'Ф', 'Х', 'Ц', 'Ч', 'Ш', 'Щ', 'Э', 'Ю', 'Я'};

char words[NUMBER][NUMBER] = {0};
char textWords[NUMBER][NUMBER] = {0};
char fragmentWords[NUMBER][NUMBER] = {0};

int getNumberWords(string str, char array[][NUMBER]);
int getNumberShingles(char textWords[][NUMBER], char fragmentWords[][NUMBER], int textCounter, int fragmentCounter);
int compareShingles(char firstString[], char secondString[]);

double antiPlagiarism(string text, string fragment);

string removeNumber(string str);
string replaceLetterToLowerCase(string str);
bool isSeparator(char symbol);

void showWords(char wordsArr[][NUMBER], int wordsCount);

int main()
{
    setlocale(LC_ALL, "Russian");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    string text = "Всем привет,6 как ваши дела,12 и как вы себя чувствуете?) Чем вы занимайтесь? Давайте сходим куда-нить погулять.";
    string fragment = "Всем привет, как ваши дела, и как вы?!";

    cout << "Percent of anti plagiarism in text " << antiPlagiarism(text, fragment) << "%" << endl;

    return 0;
}

double antiPlagiarism(string text, string fragment)
{
    text = removeNumber(text);
    text = replaceLetterToLowerCase(text);
    fragment = removeNumber(fragment);
    fragment = replaceLetterToLowerCase(fragment);

    int textCounter = getNumberWords(text, textWords);
    int fragmentCounter = getNumberWords(fragment, fragmentWords);

    showWords(textWords, textCounter);
    int shingleCounter = getNumberShingles(textWords, fragmentWords, textCounter, fragmentCounter);

    return double(shingleCounter) * 100.0 / (fragmentCounter - LENGTH_SHINGLE + 1);
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

int getNumberWords(string str, char array[][NUMBER])
{
    int wordsCounter = 0, wordsPointer = 0;

    for (int i = 0; str[i] != '\0'; i++)
    {
        if (!isSeparator(str[i]) && !isSeparator(str[i + 1]))
        {
            array[wordsCounter][wordsPointer++] = str[i];
        }

        if (!isSeparator(str[i]) && isSeparator(str[i + 1]))
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

int getNumberShingles(char textWords[][NUMBER], char fragmentWords[][NUMBER], int textCounter, int fragmentCounter)
{
    int shingleCountText = (textCounter - LENGTH_SHINGLE) + 1;
    int shingleCountFragment = (fragmentCounter - LENGTH_SHINGLE) + 1;
    int counter = 0;

    for (int i = 0; i < shingleCountFragment; i++) // shingles of fragment
    {
        for (int j = 0; j < shingleCountText; j++) // shingles of text
        {
            bool isSame = true;

            for (int n = 0; n < LENGTH_SHINGLE; n++) // every word in shingle
            {
                if (compareShingles(fragmentWords[i + n], textWords[j + n])) // compare every letter in word
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

int compareShingles(char firstString[], char secondString[])
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

string removeNumber(string str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] >= '0' && str[i] <= '9')
        {
            str[i] = ' ';
        }
    }

    return str;
}

string replaceLetterToLowerCase(string str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        for (int j = 0; upperCaseLetters[j] != '\0'; j++)
        {
            if (str[i] == upperCaseLetters[j])
            {
                str[i] = lowerCaseLetters[j];
            }
        }
    }

    return str;
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
