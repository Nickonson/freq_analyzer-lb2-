#define  unsInt unsigned int
#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#define steps_in_memory 30
#define textSize 10000
#define alpSize 32
unsInt doneThings = 0;
using namespace std;

const wchar_t * sortedAlph = L"оеаинтсрвлкмдпуяыьгзбчйхжшюцщэфъ\0\0";

struct wchInfo
{
    wchar_t chr;
    unsInt count;
};

struct unCryptInfo
{
    wchar_t * word;
    unsInt count;
};

void clearscr ( void );

bool isLetter(wchar_t symbol);

int letterNumb(wchar_t symbol);

wint_t toUpperCase(wchar_t wchr);

wint_t toLowerCase(wchar_t wchr);

unsInt unDeCryptNumber(wchar_t * word);

void usrInterface(unsInt * current_step, wchInfo cryptAlp[], wchar_t actTxt[][textSize], unsInt realTextSize, unsInt lettNumb, char outFName[]);
void outPossReplace(wchInfo cryptAlp[], unsInt realTextSize);
void Undo(unsInt * current_step);
void outWordsByLen(unsInt * current_step, wchar_t actTxt[][textSize], unsInt realTextSize);
void outByUncrypt(unsInt * current_step, wchar_t actTxt[][textSize], unsInt realTextSize);
void manualReplace(wchInfo cryptAlp[], unsInt * current_step, wchar_t actTxt[][textSize], unsInt realTextSize);
void replace(unsInt * current_step, wchar_t actTxt[][textSize], unsInt realTextSize, wint_t oldSymbol, wchar_t newSymbol);
void outActTextCond(wchar_t actTxt[][textSize], unsInt * current_step, unsInt realTextSize);
void autoDecrypt(wchar_t actTxt[][textSize], unsInt * current_step, unsInt realTextSize, wchInfo cryptAlp[]);
void outToFile(char outFName[], wchar_t actTxt[][textSize], unsInt * current_step, unsInt realTextSize);
void printDivLine();

int main()
{
    setlocale(LC_ALL, "ru_Ru");
 
    wchar_t history[steps_in_memory][textSize];

    setlocale(LC_ALL, "ru_RU");
    FILE *fin = fopen("oib/lab 2/input.txt", "r");
    char outFName[] = "oib/lab 2/output.txt";

    wchInfo freqOfSymb[alpSize];
    for(int i = 0; i < alpSize; i++)
    {
        freqOfSymb[i].count = 0;
        freqOfSymb[i].chr = L'а' + i;
    }

    unsInt count, sizeOfText, letNumb = 0;
    unsInt cur_st = 0, i;
    for(i = 0; (history[cur_st][i] = getwc(fin)) != WEOF; i++)
    {
        if(history[cur_st][i] == L'ё' || history[cur_st][i] == L'Ё')
            history[cur_st][i] = L'Е';
        else
            history[cur_st][i] = toUpperCase(history[cur_st][i]);
        //wprintf(L"%lc", history[cur_st][i]);
    }
    sizeOfText = i;
    history[cur_st][i] = '\0';
    history[cur_st][i+1] = '\0';

    fseek(fin, 0, SEEK_SET);
    //countin number of each letter meetin
    for(unsigned int counter = 0; counter < sizeOfText; counter++)
    {
        if(isLetter(history[cur_st][counter]))
            freqOfSymb[letterNumb(history[cur_st][counter])].count++;
    }

    //sorting
    for(unsigned int chng_it = 0; chng_it < alpSize - 1; chng_it++)
    {
        wchInfo tempOne;
        int t = chng_it;
        for(unsigned int counter = chng_it + 1; counter < alpSize; counter++)
        {
            if(freqOfSymb[t].count < freqOfSymb[counter].count)
                t = counter;
        }
        tempOne.chr = freqOfSymb[t].chr;
        tempOne.count = freqOfSymb[t].count;
        freqOfSymb[t].chr = freqOfSymb[chng_it].chr;
        freqOfSymb[t].count = freqOfSymb[chng_it].count;
        freqOfSymb[chng_it].chr = tempOne.chr;
        freqOfSymb[chng_it].count = tempOne.count; 
    
        letNumb += freqOfSymb[chng_it].count;
    }
    letNumb += freqOfSymb[i - 1].count;
    usrInterface(&cur_st, freqOfSymb, history, sizeOfText, letNumb, outFName);
    fclose(fin);
}

void clearscr ( void )
{
    printf("\033[2J\033[1;1H");
}

bool isLetter(wchar_t symbol)
{
    if(symbol >= L'А' && symbol <= L'Я' ||
        symbol >= L'а' && symbol <= L'я')
        return true;
    return false;
}

int letterNumb(wchar_t symbol)
{
    if(symbol >= L'А' && symbol <= L'Я')
        symbol -= (int)(L'А');
    else if(symbol >= L'а' && symbol <= L'я')
        symbol -= (int)(L'а');
    return symbol;
}

wint_t toUpperCase(wchar_t wchr)
{
    if(wchr >= L'а' && wchr <= L'я')
        wchr = L'А' + wchr - L'а';
    return wchr;
}

wint_t toLowerCase(wchar_t wchr)
{
    if(wchr >= L'А' && wchr <= L'Я')
        wchr = L'а' + wchr - L'А';
    return wchr;
}

unsInt unDeCryptNumber(wchar_t * word)
{
    unsInt out = 0;
    for(int i = 0; *(word + i) != L'\0'; i++)
    {
        if(word[i] >= L'А' && word[i] <= L'Я')
            out++;
    }
    return out;
}

void usrInterface(unsInt * current_step, wchInfo cryptAlp[], wchar_t actTxt[][textSize], unsInt realTextSize, unsInt lettNumb, char outFName[])
{
    unsigned short usrAns = 8;
    while(usrAns != 0)
    {
        while(usrAns > 7)
        {
            printf("1 | Вывод предполагаемых замен\n");
            printf("2 | Отменить последние изменения\n");
            printf("3 | Вывод слов по длине\n");
            printf("4 | Вывод слов по дешифровке\n");
            printf("5 | Вывод текста\n");
            printf("6 | Автозамена букв\n");
            printf("7 | Замена букв ручками\n");
            printf("0 | Запись рез-ов и выход\n");
            scanf("%hd", &usrAns);
        }
        switch(usrAns)
        {
            case 1:
                printDivLine();
                outPossReplace(cryptAlp, lettNumb);
                printDivLine();
                break;
            case 2:
                Undo(current_step);
                printDivLine();
                break;
            case 3:
                printDivLine();
                outWordsByLen(current_step, actTxt, realTextSize);
                printDivLine();
                break;
            case 4:
                printDivLine();
                outByUncrypt(current_step, actTxt, realTextSize);
                printDivLine();
                break;
            case 5:
                printDivLine();
                outActTextCond(actTxt, current_step, realTextSize);
                printDivLine();
                break;
            case 6:
                printDivLine();
                autoDecrypt(actTxt, current_step, realTextSize, cryptAlp);
                printDivLine();
                outActTextCond(actTxt, current_step, realTextSize);
                printDivLine();
                break;
            case 7:
                printDivLine();
                manualReplace(cryptAlp, current_step, actTxt, realTextSize);
                printDivLine();
                break;
            case 0:
                outToFile(outFName, actTxt, current_step, realTextSize);
                break;
            default:
                printf("wrong number!\n");
                break;
        }
        if(usrAns != 0)
            usrAns = 8;
    }
}

void outPossReplace(wchInfo cryptAlp[], unsInt lettNumb)
{
    printf("possible replacements : \n");
    for(int i = 0; i < alpSize; i++)
    {
        long double temp = (long double)(cryptAlp[i].count) / lettNumb;
        wprintf(L"%lc | ", cryptAlp[i].chr);
        wprintf(L"%u / %u  ( %Lf ) >===> ", cryptAlp[i].count, lettNumb, temp);
        wprintf(L"%lc\n", sortedAlph[i]);
        
    }
}

void Undo(unsInt * current_step)
{
    if(*current_step == 0)
        *current_step = steps_in_memory - 1;
    else
        *current_step -= 1;
}

void outWordsByLen(unsInt * current_step, wchar_t actTxt[][textSize], unsInt realTextSize)
{
    wchar_t * words[realTextSize / 2];
    wchar_t tempText[realTextSize];
    wchar_t * buffer;
    wcscpy(tempText, actTxt[*current_step]);
    wchar_t sep[] = L" «».,-;:!?\"\'";
    words[0] = wcstok(tempText, sep, &buffer);
    unsInt s = 1;
    while(buffer != NULL)
    {
        words[s] = wcstok(NULL, sep, &buffer);
        s++;
    }
    
    for(unsInt i = 0; i < s - 1; i++)
    {
        buffer = words[i];
        unsInt t = i;
        for(unsInt j = i + 1; j < s; j++)
        {
            if(wcslen(buffer) < wcslen(words[j]))
            {
                buffer = words[j];
                t = j;
            }
        }
        words[t] = words[i];
        words[i] = buffer;
    }
    for(unsInt i = 0; i < s; i++)
        wprintf(L"%ls\n", words[i]);
}

void outByUncrypt(unsInt * current_step, wchar_t actTxt[][textSize], unsInt realTextSize)
{
    unCryptInfo wordsInfo[realTextSize / 2];
    wchar_t tempText[realTextSize];
    wchar_t * buffer;
    unsInt buff_count;
    wcscpy(tempText, actTxt[*current_step]);
    wchar_t sep[] = L" «».,;:!?\"\'";
    wordsInfo[0].word = wcstok(tempText, sep, &buffer);
    wordsInfo[0].count = unDeCryptNumber(wordsInfo[0].word);
    unsInt s = 1;
    while(buffer != NULL)
    {
        wordsInfo[s].word = wcstok(NULL, sep, &buffer);
        if(wordsInfo[s].word != NULL)
            wordsInfo[s].count = unDeCryptNumber(wordsInfo[s].word);
        s++;
    }
    for(unsInt i = 0; i < s - 1; i++)
    {
        buffer = wordsInfo[i].word;
        buff_count = wordsInfo[i].count;
        unsInt t = i;
        for(unsInt j = i + 1; j < s; j++)
        {
            if(buff_count > wordsInfo[j].count)
            {
                buffer = wordsInfo[j].word;
                buff_count = wordsInfo[j].count;
                t = j;
            }
        }
        wordsInfo[t].word = wordsInfo[i].word;
        wordsInfo[t].count = wordsInfo[i].count;
        wordsInfo[i].word = buffer;
        wordsInfo[i].count = buff_count;
    }
    for(unsInt i = 0; i < s; i++)
    {
        wprintf(L"%30ls", wordsInfo[i].word);
        if(i % 4 == 0 && i != 0)
            printf("\n");
    }
}

void manualReplace(wchInfo cryptAlp[], unsInt * current_step, wchar_t actTxt[][textSize], unsInt realTextSize)
{
    printf("\n\n");
    short int usrAns = -1;
    wchar_t ciph = 0, unCiph = 0;
    
    while(usrAns != 3)
    {
        outActTextCond(actTxt, current_step, realTextSize);
        printDivLine();

        wprintf(L"1 | Ручной выбор\n");
        if(doneThings > 0)
            wprintf(L"2 | Отменить последнее изменение\n");
        wprintf(L"3 | Вернуться\n");
        wscanf(L" %hd", &usrAns);

        switch(usrAns)
        {
            case 1:
                wprintf(L"Буква, подлежащая замене : ");
                wscanf(L" %lc", &ciph);
                wprintf(L"Буква заменяющая : ");
                wscanf(L" %lc", &unCiph);
            
                replace(current_step, actTxt, realTextSize, ciph, unCiph);
                doneThings++;
                break;
            case 2:
                if(doneThings > 0)
                {
                    Undo(current_step);
                    doneThings--;
                }
                break;
            case 3:
                break;
            default:
                break;
        }
    }
}

void replace(unsInt * current_step, wchar_t actTxt[][textSize], unsInt realTextSize, wint_t oldSymbol, wchar_t newSymbol)
{
    unsInt ftrStep = (*current_step + 1) % steps_in_memory;
    bool changeBigLett = false;
    if(oldSymbol >= L'А' && oldSymbol <= L'Я')
        changeBigLett = true;
    if(oldSymbol >= L'А' && oldSymbol <= L'Я')
        oldSymbol -= L'А';
    else if(oldSymbol >= L'а' && oldSymbol <= L'я')
        oldSymbol -= L'а';

    for(unsInt i = 0; i < realTextSize; i++)
    {
        if(changeBigLett == true && actTxt[*current_step][i] == (L'А' + oldSymbol))
            actTxt[ftrStep][i] = newSymbol;
        else if(changeBigLett == false && actTxt[*current_step][i] == (L'а' + oldSymbol))
            actTxt[ftrStep][i] = newSymbol;
        else
            actTxt[ftrStep][i] = actTxt[*current_step][i];
    }
    *current_step = ftrStep;
}

void outActTextCond(wchar_t actTxt[][textSize], unsInt * current_step, unsInt realTextSize)
{
    for(unsInt i = 0; i < realTextSize; i++)
        wprintf(L"%lc", actTxt[*current_step][i]);
    
    printf("\n");
}

void autoDecrypt(wchar_t actTxt[][textSize], unsInt * current_step, unsInt realTextSize, wchInfo cryptAlp[])
{
    wchar_t oldSymbol, newSymbol;
    unsInt ftrStep = (*current_step + 1) % steps_in_memory;
    for(unsInt t = 0; t < realTextSize; t++)
        actTxt[ftrStep][t] = toLowerCase(actTxt[*current_step][t]);
    
    
    for(unsInt i = 0; i < alpSize && cryptAlp[i].count > 0; i++)
    {
        oldSymbol = cryptAlp[i].chr;
        newSymbol = sortedAlph[i];
        if(oldSymbol >= L'А' && oldSymbol <= L'Я')
            oldSymbol -= L'А';
        else if(oldSymbol >= L'а' && oldSymbol <= L'я')
            oldSymbol -= L'а';

        for(unsInt t = 0; t < realTextSize; t++)
        {
            if(actTxt[ftrStep][t] == (L'а' + oldSymbol))
                actTxt[ftrStep][t] = toUpperCase(newSymbol);
        }
    }
    *current_step = ftrStep;
    doneThings++;
}

void outToFile(char outFName[], wchar_t actTxt[][textSize], unsInt * current_step, unsInt realTextSize)
{
    FILE *fout = fopen(outFName, "w");
    for(unsInt i = 0; i < realTextSize; i++)
        fwprintf(fout, L"%lc", actTxt[*current_step][i]);
    fclose(fout);
}

void printDivLine()
{
    printf("\n_______________________________________\n");
}