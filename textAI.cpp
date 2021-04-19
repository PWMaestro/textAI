#include <iostream>
#include <string>
#include <Windows.h>

using namespace std;

#define EMPTY_STRING ""
#define ZERO_SYMBOL '\0'
#define NUMBER 1024
#define LENGTH_SHINGLE 3
#define LENGTH_MAX_WORD 32
#define LENGTH_MAX_FRAGMENT 256

const string separator = "./,+-#$%^&*()=!?“”— ";
const string numbers = "0123456789";

// int strCmp(char firstString[], char secondString[]);
// double compareShingles(char textWords[][NUMBER], char fragmentWords[][NUMBER], int textCounter, int fragmentCounter);
// bool isNumber(char symbol);
// void showWords(char wordsArr[][NUMBER], int wordsCount);

double antiPlagiarism(string text, string fragment);

string subString(const string &str, const int &startPosition, const int & length);

bool isEqualShingles(string fragment1[], string shingle[]);
bool isEmptyWord(const string &word);
bool isSeparator(char symbol);

void findWord(string &str, const string &text, const double &length, int &startPosition);
void parseFragment(const string &fragment, string outputArr[]);
void shiftArray(string array[], const string &newValue, const int &arrLength);

int strLen(const string &str); // what about return double?

int main()
{
    SetConsoleCP(1251);       // Set Cyrillic encoding console input
    SetConsoleOutputCP(1251); // Set Cyrillic encoding console output

    string text = "Всем привет, как ваши дела, и как вы себя чувствуете?) Чем вы занимайтесь? Давайте сходим куда-нить погулять.";
    string fragment = "Всем привет, каtк ваши дела, и как вы се9бя чувствуете!";

    cout << "Percent of anti plagiarism in text " << antiPlagiarism(text, fragment) << "%" << endl;
    return 0;
}

double antiPlagiarism(string text, string fragment)
{
    string parsedFragment[LENGTH_MAX_FRAGMENT];

    string shingle[LENGTH_SHINGLE];
    int textPointer = 0,
        wordPointer = 0,
        equalShinglesCounter = 0;
    double textLength = strLen(text);

    parseFragment(fragment, parsedFragment);

    while (textPointer < textLength)
    {
        string word(LENGTH_MAX_WORD, ZERO_SYMBOL);
        findWord(word, text, textLength, textPointer);
        // every word check here!
        if (!isEmptyWord(word))
        {
            if (wordPointer > LENGTH_SHINGLE - 1)
            {
                if (isEqualShingles(parsedFragment, shingle))
                {
                    equalShinglesCounter++;
                }
                shiftArray(shingle, word, LENGTH_SHINGLE);
            }
            else
            {
                shingle[wordPointer++] = word;
            }
        }
    }
    return equalShinglesCounter * 100.0 / 8;
}

void shiftArray(string array[], const string &newValue, const int &arrLength)
{
    for (int i = 0; i < arrLength - 1; i++)
    {
        array[i] = array[i + 1];
    }
    array[arrLength - 1] = newValue;
}

bool isEqualShingles(string fragment1[], string shingle[])
{
    // shingles counting in fragment1 array
    int totalFragWords = 0;
    for (int i = 0; fragment1[i] != EMPTY_STRING; i++)
    {
        totalFragWords++;
    }
    int shinglesTotalCount = totalFragWords - LENGTH_SHINGLE + 1;

    // compare shingles
    for (int i = 0; i < shinglesTotalCount; i++) // whole shingle
    {
        bool isWordsSame = true;

        for (int j = 0; j < LENGTH_SHINGLE; j++) // just one word
        {
            if (shingle[j] != fragment1[i + j])
            {
                isWordsSame = false;
            }
        }
        if (isWordsSame)
        {
            return true;
        }
    }
    // cout << shingle[0] << ' ' << shingle[1] << ' ' << shingle[2] << endl;
    return false;
}

void parseFragment(const string &fragment, string outputArr[])
{
    int textLength = strLen(fragment),
        textPointer = 0,
        wordPointer = 0;

    while (textPointer < textLength)
    {
        string word(LENGTH_MAX_WORD, ZERO_SYMBOL);
        findWord(word, fragment, textLength, textPointer);
        // every word check here!
        if (!isEmptyWord(word))
        {
            outputArr[wordPointer++] = subString(word, 0, strLen(word));
        }
    }
}

int strLen(const string &str)
{
    int counter = 0;

    for (int i = 0; str[i] != '\0'; i++)
    {
        counter++;
    }
    return counter;
}

string subString(const string &str, const int &startPosition, const int &length)
{
    string output(length, '\0');

    for (int i = 0, j = startPosition; i < length; i++, j++)
    {
        output[i] = str[j];
    }
    return output;
}

void findWord(string &str, const string &text, const double &length, int &startPosition)
{
    int wordBegin = -1, wordLength = 0;

    for (int i = startPosition; i < length; i++, startPosition++)
    {
        if (wordBegin == -1 && !isSeparator(text[i]))
        {
            wordBegin = i;
        }
        if (wordBegin != -1 && !isSeparator(text[i]))
        {
            wordLength++;
        }
        if (wordBegin != -1 && isSeparator(text[i]))
        {
            break;
        }
    }
    str = subString(text, wordBegin, wordLength);
}

// int getNumberPartitionedWords(string str, char array[][NUMBER])
// {
//     int wordsCounter = 0, wordsPointer = 0;

//     for (int i = 0; str[i] != '\0'; i++)
//     {
//         if (!isSeparator(str[i]) && !isSeparator(str[i + 1]))
//         {
//             array[wordsCounter][wordsPointer++] = str[i];
//         }

//         if (!isSeparator(str[i]) && (isSeparator(str[i + 1])))
//         {
//             array[wordsCounter++][wordsPointer] = str[i];
//             wordsPointer = 0;
//         }

//         if (!isSeparator(str[i]) && str[i + 1] == '\0')
//         {
//             array[++wordsCounter][wordsPointer] = str[i];
//             wordsPointer = 0;
//         }
//     }

//     return wordsCounter;
// }

// int strCmp(char firstString[], char secondString[])
// {
//     for (int i = 0; firstString[i] != '\0'; i++)
//     {
//         if (firstString[i] > secondString[i])
//         {
//             return 1;
//         }

//         if (firstString[i] < secondString[i])
//         {
//             return -1;
//         }
//     }

//     return 0;
// }

// double compareShingles(char textWords[][NUMBER], char fragmentWords[][NUMBER], int textCounter, int fragmentCounter)
// {
//     int shingleCountText = (textCounter - LENGTH_SHINGLE) + 1;
//     int shingleCountFragment = (fragmentCounter - LENGTH_SHINGLE) + 1;
//     double counter = 0;

//     for (int i = 0; i < shingleCountFragment; i++) // shingles of fragment
//     {
//         for (int j = 0; j < shingleCountText; j++) // shingles of text
//         {
//             bool isSame = true;

//             for (int n = 0; n < LENGTH_SHINGLE; n++) // every word in shingle
//             {
//                 if (strCmp(fragmentWords[i + n], textWords[j + n])) // compare every letter in word
//                 {
//                     isSame = false;
//                     break;
//                 }
//             }
//             if (isSame)
//             {
//                 counter++;
//                 break;
//             }
//             else
//             {
//                 continue;
//             }
//         }
//     }
//     return counter;
// }

// void showWords(char wordsArr[][NUMBER], int wordsCount)
// {
//     for (int i = 0; i < wordsCount; i++)
//     {
//         for (int j = 0; wordsArr[i][j] != '\0'; j++)
//         {
//             cout << wordsArr[i][j];
//         }
//         cout << endl;
//     }
// }

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

bool isEmptyWord(const string &word)
{
    return word[0] == '\0';
}

// bool isOutOfRange(const int &value, const int &rangeStart, const int &rangeEnd)
// {
//     1
// }

// bool isNumber(char symbol)
// {
//     for (int i = 0; numbers[i] != '\0'; i++)
//     {
//         if (symbol == numbers[i])
//         {
//             return false;
//         }
//     }

//     return true;
// }
