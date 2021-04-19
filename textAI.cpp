#include <iostream>
// #include <cstring>
#include <string>
#include <Windows.h>

using namespace std;

#define EMPTY_STRING ""
#define NUMBER 1024
#define LENGTH_SHINGLE 3
#define LENGTH_MAX_WORD 32
#define LENGTH_MAX_FRAGMENT 256

const string separator = "./,+-#$%^&*()=!?“”— ";
const string numbers = "0123456789";

char words[NUMBER][NUMBER] = { 0 };
char textWords[NUMBER][NUMBER] = { 0 };
char fragmentWords[NUMBER][NUMBER] = { 0 };

// my new array for fragment
string fragment1[LENGTH_MAX_FRAGMENT];

int getNumberPartitionedWords(string str, char array[][NUMBER]);
int strCmp(char firstString[], char secondString[]);
// also new function
int strLen(const string &str);

double antiPlagiarism(string text, string fragment);
double compareShingles(char textWords[][NUMBER], char fragmentWords[][NUMBER], int textCounter, int fragmentCounter);

bool isSeparator(char symbol);
bool isNumber(char symbol);

void showWords(char wordsArr[][NUMBER], int wordsCount);


// myNewFunctions
void findWord(string &str, const string &text, const double &length, int &startPosition);
void streamingTextProcessing(const string &text);
string subString(const string &str, const int &startPosition, const int & length);
bool isEqualShingles(const string shingle[]);
void parseFragment(const string &fragment, string wordsArray[]);
void shiftArray(string array[], const string &newValue, const int &arrLength);

int main()
{
    SetConsoleCP(1251);       // Set Cyrillic encoding console input
    SetConsoleOutputCP(1251); // Set Cyrillic encoding console output

    string text = "Всем привет, как ваши дела, и как вы себя чувствуете?) Чем вы занимайтесь? Давайте сходим куда-нить погулять.";
    string fragment = "Всем привет, каtк ваши дела, и как вы се9бя чувствуете!";

    parseFragment(fragment, fragment1);
    streamingTextProcessing(text);
    // cout << "Percent of anti plagiarism in text " << antiPlagiarism(text, fragment) << "%" << endl;

    return 0;
}

double antiPlagiarism(string text, string fragment)
{
    // parseFragment(fragment, fragment1);
    // streamingTextProcessing(text);
    double textCounter = getNumberPartitionedWords(text, textWords);
    double fragmentCounter = getNumberPartitionedWords(fragment, fragmentWords);

    showWords(textWords, textCounter);
    showWords(fragmentWords, fragmentCounter);

    double shingleCounter = compareShingles(textWords, fragmentWords, textCounter, fragmentCounter);

    return shingleCounter * 100.0 / (fragmentCounter - LENGTH_SHINGLE + 1);
    // return 0.0;
}

void streamingTextProcessing(const string &text)
{
    double textLength = strLen(text);
    int textPointer = 0,
        shinglePointer = 0,
        equalShinglesCounter = 0;
    string shingle[LENGTH_SHINGLE];

    while (textPointer < textLength)
    {
        string word(LENGTH_MAX_WORD, '\0');
        findWord(word, text, textLength, textPointer);
        // every word check here!
        if (word[0] != '\0') // checking empty string - find word can throw empty string at the end
        {
            if (shinglePointer > LENGTH_SHINGLE - 1)
            {
                if (isEqualShingles(shingle))
                {
                    equalShinglesCounter++;
                }
                shiftArray(shingle, word, LENGTH_SHINGLE);
            }
            else
            {
                shingle[shinglePointer++] = word;
            }
            // cout << word << ' ' << textPointer << endl;
        }
    }
    cout << "Plagiarism: " << equalShinglesCounter * 100.0 / 8 << '%' << endl;
}

void shiftArray(string array[], const string &newValue, const int &arrLength)
{
    for (int i = 0; i < arrLength - 1; i++)
    {
        array[i] = array[i + 1];
    }
    array[arrLength - 1] = newValue;
}

bool isEqualShingles(const string shingle[])
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

void parseFragment(const string &fragment, string wordsArray[])
{
    int textLength = strLen(fragment),
        textPointer = 0,
        wordPointer = 0;
    // int equalShinglesCounter = 0;
    // string shingle[LENGTH_SHINGLE];

    while (textPointer < textLength)
    {
        string word(LENGTH_MAX_WORD, '\0');
        findWord(word, fragment, textLength, textPointer);
        // every word check here!
        if (word[0] != '\0') // checking empty string - find word can throw empty string at the end
        {
            // cout << strLen(word) << endl;
            wordsArray[wordPointer++] = subString(word, 0, strLen(word));
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
    double counter = 0;

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
            if (isSame)
            {
                counter++;
                break;
            }
            else
            {
                continue;
            }
        }
    }
    return counter;
}

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
