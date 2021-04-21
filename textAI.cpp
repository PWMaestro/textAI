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
#define SEPARATORS "./,+-#$%^&*()=!?УФЧ "
#define ENG_LETTERS "AaBCcEeHKkMOoPpTXx"
#define RUS_LETTERS "ја¬сс≈еЌ кћќо–р“’х"
#define RUS_LETTERS_LOWER_CASE "абвгдеЄжзийклмнопрстуфхцчшщэю€"
#define RUS_LETTERS_UPPER_CASE "јЅ¬√ƒ≈®∆«»… ЋћЌќѕ–—“”‘’÷„ЎўЁёя"

const string badWords[] = { "чтд", "либо", "или", "что", "чтобы", "как", "нибудь", "только", "зато", "также", "когда", "чем"};

double antiPlagiarism(string text, string fragment);
string subString(const string &str, const int &startPosition, const int &length);

int compareStrings(const string &str1, const string &str2);
int getMaxStringLength(const string &string1, const string &string2);
int strLen(const string &str); // what about return double?

void findWord(string &str, const string &text, int &startPosition, const int &length);
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

    string text = "¬сем привет, как ваши дела, и как вы себ€ чувствуете?) „ем вы занимаетесь? ƒавайте сходим куда-нибудь погул€ть.";
    string fragment = "¬сем привет что-ли =)  ак же ваши дела? » как вы себ€ чувствуете!";

    cout << "Percent of anti plagiarism in text " << antiPlagiarism(text, fragment) << "%" << endl;
    return 0;
}

double antiPlagiarism(string text, string fragment)
{
    string parsedFragment[LENGTH_MAX_FRAGMENT];

    string shingle[LENGTH_SHINGLE];
    int wordPointer = 0,
        textPointer = 0,
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
        findWord(word, text, textPointer, textLength);
        int length = strLen(word);

        if (!isEmptyWord(word))
        {
            replaceLetter(word, length, ENG_LETTERS, RUS_LETTERS);
            replaceLetter(word, length, RUS_LETTERS_UPPER_CASE, RUS_LETTERS_LOWER_CASE);
            if ( length < 3 || isNumber(word, length) || isBadWord(word) )
            {
                continue;
            }      
            if (wordPointer > 0 && !compareStrings(shingle[wordPointer - 1], word))
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
        findWord(word, fragment, textPointer, textLength);
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
    str = subString(text, wordBegin, wordLength);
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
    int stringLength1 = strLen(string1),
        stringLength2 = strLen(string2);
    return (stringLength1 > stringLength2) ? stringLength1 : stringLength2;
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
        if ( !compareStrings(word, badWords[i]) )
        {
            return true;
        }
    }
    return false;
}