#include <iostream>
#include <string>
#include <Windows.h>

using namespace std;

#define EMPTY_STRING ""
#define ZERO_SYMBOL '\0'
#define LENGTH_SHINGLE 3
#define LENGTH_MIN_WORD 3
#define LENGTH_MAX_WORD 32
#define LENGTH_EXCLUSIONS 12
#define LENGTH_MAX_FRAGMENT 256
#define SEPARATORS "./,+-#$%^&*( )=!?У'ФЧ"
#define ENG_LETTERS "AaBCcEeHKkMOoPpTXx"
#define RUS_LETTERS "ја¬сс≈еЌ кћќо–р“’х"
#define RUS_LETTERS_LOWER_CASE "абвгдеЄжзийклмнопрстуфхцчшщэю€"
#define RUS_LETTERS_UPPER_CASE "јЅ¬√ƒ≈®∆«»… ЋћЌќѕ–—“”‘’÷„ЎўЁёя"

const string exclusions[] = { "чтд", "либо", "или", "что", "чтобы", "как", "нибудь", "только", "зато", "также", "когда", "чем"};

double antiPlagiarism(string text, string fragment);
string getSubstirng(const string &originString, const int &startPosition, const int &length);

int compareStrings(const string &str1, const string &str2);
int getMaxStringLength(const string &string1, const string &string2);
int getStringLength(const string &originString);
int getShinglesTotalCount(const string parsedFragment[]);

void findWord(string &str, const string &text, int &startPosition, const int &length);
void parseFragment(const string &fragment, string outputArr[]);
void shiftQueue(string queue[], const int &queueLength, const string &newElement);
void replaceLetter(string &word, const int &length, const string &checkedLetter, const string &replaceableLetter);
void replaceUppercaseLetters(string &word, const int &length);
void replaceEngLetters(string &word, const int &length);


bool isMatchesInFragment(const string shingle[], const string textFragment[], const int &shinglesCount);
bool isEqualShingles(const string shingle[], const string textFragment[], const int &startPosition);
bool isRepeated(const string &word, const string shingle[], const int &currentWordPointer);
bool isEmptyWord(const string &word);
bool isSeparator(char symbol);
bool isNumeral(char symbol);
bool isExclusion(const string &word);
bool isNumber(const string &string, const int &length);

int main()
{
    SetConsoleCP(1251);       // Set Cyrillic encoding console input
    SetConsoleOutputCP(1251); // Set Cyrillic encoding console output

    string text = "¬сем привет, как ваши дела, и как вы себ€ чувствуете?) „ем вы занимаетесь? ƒавайте сходим куда-нибудь погул€ть.";
    string fragment = "¬сем привет что-ли =)  ак же ваши дела '97? » как вы себ€ чувствуете!";

    cout << "Percent of anti plagiarism in text " << antiPlagiarism(text, fragment) << "%" << endl;
    return 0;
}

double antiPlagiarism(string text, string fragment)
{
    string parsedFragment[LENGTH_MAX_FRAGMENT];
    string shingle[LENGTH_SHINGLE];
    int textLength = getStringLength(text),
        wordPointer = 0,
        textPointer = 0,
        equalShinglesCounter = 0;

    parseFragment(fragment, parsedFragment);
    
    int shinglesTotalCount = getShinglesTotalCount(parsedFragment);

    while (textPointer < textLength)
    {
        string word(LENGTH_MAX_WORD, ZERO_SYMBOL);
        findWord(word, text, textPointer, textLength);
        int length = getStringLength(word);

        if (isEmptyWord(word) || length < LENGTH_MIN_WORD || isNumber(word, length))
        {
            continue;
        }
        replaceEngLetters(word, length);
        replaceUppercaseLetters(word, length);
        if (isExclusion(word) || isRepeated(word, shingle, wordPointer))
        {
            continue;
        }
        else if (wordPointer < LENGTH_SHINGLE)
        {
            shingle[wordPointer++] = getSubstirng(word, 0, length);
        }
        else if (isMatchesInFragment(shingle, parsedFragment, shinglesTotalCount))
        {
            shiftQueue(shingle, LENGTH_SHINGLE, word);
            equalShinglesCounter++;
        }
        cout << word << endl;
    }
    return equalShinglesCounter * 100.0 / shinglesTotalCount;
}

bool isMatchesInFragment(const string shingle[], const string textFragment[], const int &shinglesCount)
{
    for (int i = 0; i < shinglesCount; i++)
    {
        if (isEqualShingles(shingle, textFragment, i))
        {
            return true;
        }
    }
    return false;
}

bool isEqualShingles(const string shingle[], const string textFragment[], const int &startPosition)
{
    for (int j = 0; j < LENGTH_SHINGLE; j++)
    {
        if (compareStrings(shingle[j], textFragment[startPosition + j]))
        {
            return false;
        }
    }
    return true;
}

void parseFragment(const string &fragment, string outputArr[])
{
    int textLength = getStringLength(fragment),
        textPointer = 0,
        wordPointer = 0;

    while (textPointer < textLength)
    {
        string word(LENGTH_MAX_WORD, ZERO_SYMBOL);
        findWord(word, fragment, textPointer, textLength);
        int length = getStringLength(word);

        if (isEmptyWord(word) || length < LENGTH_MIN_WORD || isNumber(word, length))
        {
            continue;
        }
        replaceEngLetters(word, length);
        replaceUppercaseLetters(word, length);
        if (isExclusion(word))
        {
            continue;
        }
        outputArr[wordPointer++] = getSubstirng(word, 0, length);
        cout << word << endl;
    }
}

int getStringLength(const string &originString)
{
    int counter = 0;
    for (int i = 0; originString[i] != '\0'; i++)
    {
        counter++;
    }
    return counter;
}

int getShinglesTotalCount(const string parsedFragment[])
{
    int wordsTotalCount = 0;
    for (int i = 0; i < LENGTH_MAX_FRAGMENT && parsedFragment[i] != EMPTY_STRING; i++)
    {
        wordsTotalCount++;        
    }
    return wordsTotalCount - LENGTH_SHINGLE + 1;
}

string getSubstirng(const string &originString, const int &startPosition, const int &length)
{
    string outputString(length, '\0');
    for (int i = 0, j = startPosition; i < length; i++, j++)
    {
        outputString[i] = originString[j];
    }
    return outputString;
}

void findWord(string &str, const string &text, int &startPosition, const int &length)
{
    int wordBegin = -1,
        wordLength = 0;
    bool isWordStarted = false;

    for (; startPosition < length; startPosition++)
    {
        if (isWordStarted)
        {
            if (isSeparator(text[startPosition]))
            {
                break;
            }
            else
            {
                wordLength++;
            }
        }
        else if (!isSeparator(text[startPosition]))
        {
            isWordStarted = true;
            wordLength++;
            wordBegin = startPosition;
        }
    }
    str = getSubstirng(text, wordBegin, wordLength);
}

int compareStrings(const string &string1, const string &string2)
{
    int maxLength = getMaxStringLength(string1, string2);

    for (int i = 0; i < maxLength; i++)
    {
        if (string1[i] > string2[i])
        {
            return 1;
        }
        else if (string1[i] < string2[i])
        {
            return -1;
        }
    }
    return 0;
}

int getMaxStringLength(const string &string1, const string &string2)
{
    int stringLength1 = getStringLength(string1),
        stringLength2 = getStringLength(string2);
    return (stringLength1 > stringLength2) ? stringLength1 : stringLength2;
}

void replaceLetter(string &word, const int &length, const char oldLetter, const char newLetter)
{
    for (int i = 0; i < length; i++)
    {
        if (word[i] == oldLetter)
        {
            word[i] = newLetter;
        }
    }
}
void replaceEngLetters(string &word, const int &length)
{
    for (int i = 0; ENG_LETTERS[i] != '\0'; i++)
    {
        replaceLetter(word, length, ENG_LETTERS[i], RUS_LETTERS[i]);
    }
}

void replaceUppercaseLetters(string &word, const int &length)
{
    for (int i = 0; RUS_LETTERS_UPPER_CASE[i] != '\0'; i++)
    {
        replaceLetter(word, length, RUS_LETTERS_UPPER_CASE[i], RUS_LETTERS_LOWER_CASE[i]);
    }
}

void shiftQueue(string queue[], const int &queueLength, const string &newElement)
{
    for (int i = 1; i < queueLength; i++)
    {
        queue[i - 1] = queue[i];
    }
    queue[queueLength - 1] = newElement;
}

bool isEmptyWord(const string &word)
{
    return word[0] == '\0';
}

bool isExclusion(const string &word)
{
    for (int i = 0; i < LENGTH_EXCLUSIONS; i++)
    {
        if (!compareStrings(word, exclusions[i]))
        {
            return true;
        }
    }
    return false;
}

bool isNumber(const string &string, const int &length)
{
    for (int i = 0; i < length; i++)
    {
        if (!isNumeral(string[i]))
        {
            return false;
        }
    }
    return true;
}

bool isRepeated(const string &word, const string shingle[], const int &currentWordPointer)
{
    return currentWordPointer > 0 && !compareStrings(shingle[currentWordPointer - 1], word);
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

bool isNumeral(char symbol)
{
    return symbol >= '0' && symbol <= '9';
}