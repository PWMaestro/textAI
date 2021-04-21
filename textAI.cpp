#include <iostream>
#include <string>
#include <Windows.h>

using namespace std;

#define EMPTY_STRING ""
#define ZERO_SYMBOL '\0'
#define LENGTH_SHINGLE 3
#define LENGTH_MAX_WORD 32
#define LENGTH_BAD_WORDS 12
#define LENGTH_MAX_FRAGMENT 256
#define SEPARATORS "./,+-#$%^&*()=!?��� "
#define ENG_LETTERS "AaBCcEeHKkMOoPpTXx"
#define RUS_LETTERS "������������������"
#define RUS_LETTERS_LOWER_CASE "�����������������������������"
#define RUS_LETTERS_UPPER_CASE "�����Ũ�����������������������"

const string badWords[] = { "���", "����", "���", "���", "�����", "���", "������", "������", "����", "�����", "�����", "���"};

double antiPlagiarism(string text, string fragment);
string subString(const string &str, const int &startPosition, const int & length);

int strCmp(const string &str1, const string &str2);
int strLen(const string &str); // what about return double?

void findWord(string &str, const string &text, const double &length, int &startPosition);
void parseFragment(const string &fragment, string outputArr[]);
void shiftArray(string array[], const string &newValue, const int &arrLength);
void replaceLetter(string &word, const int &length, const string &checkedLetter, const string &replaceableLetter);

bool isEqualShingles(string fragment1[], string shingle[]);
bool isEmptyWord(const string &word);
bool isSeparator(char symbol);
bool isNumeral(char symbol);
bool isBadWord(const string &word);
bool isNumber(const string &str, const int &length);

int main()
{
    SetConsoleCP(1251);       // Set Cyrillic encoding console input
    SetConsoleOutputCP(1251); // Set Cyrillic encoding console output

    string text = "���� ������, ��� ���� ����, � ��� �� ���� ����������?) ��� �� �����������? ������� ������ ����-������ ��������.";
    string fragment = "���� ������ ���-�� =) ��� �� ���� ����? � ��� �� ���� ����������!";

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

    int wordsTotalCount = 0;
    for (int i = 0; i < LENGTH_MAX_FRAGMENT && parsedFragment[i] != EMPTY_STRING; i++)
    {
        wordsTotalCount++;        
    }
    int shinglesTotalCount = wordsTotalCount - LENGTH_SHINGLE + 1;

    while (textPointer < textLength)
    {
        string word(LENGTH_MAX_WORD, ZERO_SYMBOL);
        findWord(word, text, textLength, textPointer);
        int length = strLen(word);

        if (!isEmptyWord(word))
        {
            replaceLetter(word, length, ENG_LETTERS, RUS_LETTERS);
            replaceLetter(word, length, RUS_LETTERS_UPPER_CASE, RUS_LETTERS_LOWER_CASE);
            if ( length < 3 || isNumber(word, length) || isBadWord(word) )
            {
                continue;
            }      
            if (wordPointer > 0 && !strCmp(shingle[wordPointer - 1], word))
            {
                continue;
            }
            if (wordPointer < LENGTH_SHINGLE)
            {
                shingle[wordPointer++] = subString(word, 0, length);
            }
            else
            {
                if (isEqualShingles(parsedFragment, shingle))
                {
                    equalShinglesCounter++;
                }
                shiftArray(shingle, word, LENGTH_SHINGLE);
            }
            cout << word << endl;
        }
    }
    return equalShinglesCounter * 100.0 / shinglesTotalCount;
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
        int length = strLen(word);

        if (!isEmptyWord(word))
        {
            replaceLetter(word, length, ENG_LETTERS, RUS_LETTERS);
            replaceLetter(word, length, RUS_LETTERS_UPPER_CASE, RUS_LETTERS_LOWER_CASE);
            if ( length < 3 || isNumber(word, length) || isBadWord(word) )
            {
                continue;
            }      
            outputArr[wordPointer++] = subString(word, 0, strLen(word));
        }
        cout << word << endl;
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

int strCmp(const string &str1, const string &str2)
{
    int strLength1 = strLen(str1),
        strLength2 = strLen(str2),
        maxLength = (strLength1 > strLength2) ? strLength1 : strLength2;

    for (int i = 0; i < maxLength; i++)
    {
        if ( str1[i] > str2[i] )
        {
            return 1;
        }
        else if ( str1[i] < str2[i] )
        {
            return -1;
        }
    }
    return 0;
}

void replaceLetter(string &word, const int &length, const string &checkedLetter, const string &replaceableLetter)
{
    for (int i = 0; i < length; i++)
    {
        for (int j = 0; checkedLetter[j] != '\0'; j++)
        {
            if (word[i] == checkedLetter[j])
            {
                word[i] = replaceableLetter[j];
            }
        }
    }
}

void shiftArray(string array[], const string &newValue, const int &arrLength)
{
    for (int i = 0; i < arrLength - 1; i++)
    {
        array[i] = array[i + 1];
    }
    array[arrLength - 1] = newValue;
}

bool isSeparator(char symbol)
{
    for (int i = 0; SEPARATORS[i] != '\0'; i++)
    {
        if (symbol == SEPARATORS[i])
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

bool isNumeral(char symbol)
{
    if (symbol < '0' || symbol > '9')
    {
        return false;
    }
    return true;
}

bool isNumber(const string &str, const int &length)
{
    for (int i = 0; i < length; i++)
    {
        if (!isNumeral(str[i]))
        {
            return false;
        }
    }
    return true;
}

bool isBadWord(const string &word)
{
    for (int i = 0; i < LENGTH_BAD_WORDS; i++)
    {
        if ( !strCmp(word, badWords[i]) )
        {
            return true;
        }
    }
    return false;
}