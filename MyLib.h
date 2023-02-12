#pragma once
#include <iostream>
#include <string>
#include <vector>
using namespace std;
namespace MyLib
{
    template <typename T>
    T ReadNumber()
    {
        T Number;
        cin >> Number;
        while (cin.fail())
        {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::digits, '\n');
            cout << "Invalid Input, Try Again.\n";
            cin >> Number;
        }
        return Number;
    }
    template <typename T>
    T ReadNumber(T From, T To)
    {
        T number;
        cin >> number;
        while (cin.fail() || (number < From || number > To))
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::digits, '\n');
            cout << "Invalid Input, Try Again.\n";
            cin >> number;
        }

        return number;
    }
    template <typename T>
    T ReadPositiveNumber()
    {
        T number;
        cin >> number;
        while (cin.fail() || number <= 0)
        {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::digits, '\n');
            cout << "Invalid Input, Try Again.\n";
            cin >> number;
        }
        return number;
    }
    string ReadString()
    {
        string str;
        getline(cin >> ws, str);
        return str;
    }
    char ReadChar()
    {
        char str;
        cin >> str;
        return str;
    }
    char InvertLetterCase(char Letter)
    {
        return isupper(Letter) ? tolower(Letter) : toupper(Letter);
    }
    bool CheckChar(char Char)
    {
        do
        {
            if (tolower(Char) == 'y')
                return true;
            else if (tolower(Char) == 'n')
                return false;
            else
            {
                cout << "Invalid Input, Try Again!\n";
                cin >> Char;
            }
        } while (Char);

    }
    string UpperString(string MyString)
    {
        for (short x = 0; x <= MyString.length(); x++)
        {
            MyString[x] = toupper(MyString[x]);
        }
        return MyString;
    }
    string LowerString(string MyString)
    {
        for (short x = 0; x <= MyString.length(); x++)
        {
            MyString[x] = tolower(MyString[x]);
        }
        return MyString;
    }
    enum enWhatToCount { SmallLetters = 0, CapitalLetters = 1, All = 2 };
    short WhatToCount(string str, enWhatToCount WhatToCount = enWhatToCount::All)
    {
        if (WhatToCount == enWhatToCount::All)
            return str.length();

        short CountLetter = 0;
        for (short i = 0; i <= str.length(); i++)
        {
            if (WhatToCount == enWhatToCount::CapitalLetters && isupper(str[i]))
                CountLetter++;

            if (WhatToCount == enWhatToCount::SmallLetters && islower(str[i]))
                CountLetter++;
        }
        return CountLetter;
    }
    void ReadArray(int Array[100], int& ArrayLength)
    {
        for (int i = 0; i < ArrayLength; i++)
        {
            cout << "Element [" << i + 1 << "] : ";
            cin >> Array[i];
        }
    }
    void PrintArrElements(int Arr[100], int ArrLength)
    {
        for (int i = 0; i < ArrLength; i++)
        {
            cout << Arr[i] << " ";
        }
        cout << endl;
    }
    int TimesRepeated(int Num, int Arr[100], int ArrLength)
    {
        short counter = 0;
        for (short i = 0; i < ArrLength; i++)
        {
            if (Num == Arr[i])
                counter++;
        }
        return counter;
    }
    int RandomNumber(int From, int To)
    {
        return rand() % (To - From + 1) + From;
    }
    void FillArrayWithRandomNumbers(int Array[100], int& ArrayLength)
    {
        for (int i = 0; i < ArrayLength; i++)
        {
            Array[i] = RandomNumber(1, 100);
        }
    }
    int MaxNumberInArray(int Arr[100], int ArrLength)
    {
        int Max = Arr[0];
        for (int i = 0; i < ArrLength; i++)
        {
            if (Arr[i] > Max)
            {
                Max = Arr[i];
            }
        }
        return Max;
    }
    int MinNumberInArray(int Arr[100], int ArrLength)
    {
        int Min = Arr[0];
        for (int i = 0; i < ArrLength; i++)
        {
            if (Arr[i] < Min)
            {
                Min = Arr[i];
            }
        }
        return Min;
    }
    int SumOfArray(int Arr[100], int ArrLength)
    {
        int Sum = 0;
        for (int i = 0; i < ArrLength; i++)
        {
            Sum += Arr[i];
        }
        return Sum;
    }
    float AverageOfArray(int Arr[100], int ArrLength)
    {
        return (float)SumOfArray(Arr, ArrLength) / ArrLength;
    }
    enum enPrimeNotPrime { Prime = 1, NotPrime = 0 };
    enPrimeNotPrime CheckPrime(int Number)
    {
        for (int i = 2; i <= round(Number / 2); i++)
        {
            if (Number % i == 0)
                return enPrimeNotPrime::NotPrime;
        }
        return enPrimeNotPrime::Prime;
    }
    void Swap(int& num1, int& num2)
    {
        int temp = 0;
        temp = num1;
        num1 = num2;
        num2 = temp;
    }
    void ShuffleArray(int Arr[100], int ArrLength)
    {
        for (short i = 0; i < ArrLength / 2; i++)
        {
            Swap(Arr[RandomNumber(1, ArrLength) - 1], Arr[RandomNumber(1, ArrLength) - 1]);
        }
    }
    void CopyArrInReverseOrder(int Arr[100], int ArrInReverse[100], int ArrLength)
    {
        for (short i = 0; i < ArrLength; i++)
        {
            ArrInReverse[i] = Arr[ArrLength - 1 - i];
        }
    }
    int FindtNumberPositionInArray(int NumToSearchFor, int Arr[100], int ArrLength)
    {
        for (int i = 0; i < ArrLength; i++)
        {
            if (Arr[i] == NumToSearchFor)
                return i;
        }
        return -1;
    }
    bool IsNumberInArray(int NumToSearchFor, int Arr[100], int ArrLength)
    {
        return FindtNumberPositionInArray(NumToSearchFor, Arr, ArrLength) != -1;
    }
    bool IsCharInArray(char L, char Arr[100], short Length)
    {
        for (short i = 0; i < Length; i++)
        {
            if (Arr[i] == L)
                return true;
        }
        return false;
    }
    void AddArrayElement(int number, int Arr[100], int& ArrLength)
    {
        ArrLength++;
        Arr[ArrLength - 1] = number;
    }
    void AddArrayElement(string Element, string Arr[100], int& ArrLength)
    {
        ArrLength++;
        Arr[ArrLength - 1] = Element;
    }
    void InputUserNumbersInArray(int Arr[100], int& ArrLength)
    {
        bool AddMorenumber = true;
        do
        {
            AddArrayElement(ReadNumber<int>(), Arr, ArrLength);
            cout << "\nDo you want to add more numbers? [0]: No, [1]: Yes? ";
            cin >> AddMorenumber;
        } while (AddMorenumber);

    }
    void CopyDistinctNumbersToArray(int Arr[10], int Arr2[10], int ArrLength, int& Arr2Length)
    {
        for (int i = 0; i < ArrLength; i++)
        {
            if (!IsNumberInArray(Arr[i], Arr2, ArrLength))
                AddArrayElement(Arr[i], Arr2, Arr2Length);
        }
    }
    enum enOddOrEven { Odd = 1, Even = 2 };
    enOddOrEven CheckOddOrEven(int Number)
    {
        return (Number % 2 != 0) ? enOddOrEven::Odd : enOddOrEven::Even;
    }
    int CountOddNumbersInArray(int Arr[100], int ArrLength)
    {
        int count = 0;
        for (int i = 0; i < ArrLength; i++)
        {
            if (CheckOddOrEven(Arr[i]) == enOddOrEven::Odd)
                count++;
        }
        return count;
    }
    int CountEvenNumbersInArray(int Arr[100], int ArrLength)
    {
        int count = 0;
        for (int i = 0; i < ArrLength; i++)
        {
            if (CheckOddOrEven(Arr[i]) == enOddOrEven::Even)
                count++;
        }
        return count;
    }
    int CountPositiveNumbersInArray(int Arr[100], int ArrLength)
    {
        int count = 0;
        for (int i = 0; i < ArrLength; i++)
        {
            if (Arr[i] >= 0)
                count++;
        }
        return count;
    }
    int CountNegativeNumbersInArray(int Arr[100], int ArrLength)
    {
        int count = 0;
        for (int i = 0; i < ArrLength; i++)
        {
            if (Arr[i] < 0)
                count++;
        }
        return count;
    }
    vector <string> SplitString(string str, string delim = " ")
    {
        vector <string> vString;
        short pos = 0;
        string word = "";
        while ((pos = str.find(delim)) != str.npos)
        {
            word = str.substr(0, pos);
            if (word != "")
            {
                vString.push_back(word);
            }
            str.erase(0, pos + delim.length());
        }
        if (str != "")
        {
            vString.push_back(str);
        }
        return vString;
    }
    string ReplaceWordInString(string S, string stringToReplace, string ReplaceTo)
    {
        short pos = S.find(stringToReplace);
        while (pos != S.npos)
        {
            S = S.replace(pos, stringToReplace.length(), ReplaceTo);
            pos = S.find(stringToReplace);
        }
        return S;
    }
}