#define  unsInt unsigned int
#include <locale.h>
#include <stdlib.h>
#include <stdio.h>

#include <iostream>

#define steps_in_memory 30
#define textSize 1000

using namespace std;

const wchar_t* sortedAlph = L"оеаинтсрвлкмдпуяыьгзбчйхжшюцщэфъ";

struct wchInfo
{
    wchar_t chr;
    unsInt count;
};

bool isLetter(wchar_t symbol);

int letterNumb(wchar_t symbol);


void usrInterface(unsInt * current_step, wchInfo cryptAlp[], wchar_t actTxt[][textSize], unsInt realTextSize, unsInt lettNumb, char outFName[]);
void outPossReplace(wchInfo cryptAlp[], unsInt realTextSize);
void Undo(unsInt * current_step);
void outWordsByLen();
void outByUncrypt();
void manualReplace();
void outActTextCond(wchar_t actTxt[][textSize], unsInt * current_step, unsInt realTextSize);
void autoDecrypt(wchar_t actTxt[][textSize], unsInt * current_step, unsInt realTextSize, wchInfo cryptAlp[]);
void outToFile(char outFName[], wchar_t actTxt[][textSize], unsInt * current_step, unsInt realTextSize);

int main()
{
    wchar_t history[steps_in_memory][textSize];

    setlocale(LC_ALL, "ru_RU");
    FILE *fin = fopen("oib/lab 2/input.txt", "r");
    char outFName[] = "oib/lab 2/output.txt";
    
    
    wchInfo freqOfSymb[33];
    for(int i = 0; i < 33; i++)
    {
        freqOfSymb[i].count = 0;
        freqOfSymb[i].chr = L'а' + i;
    }
    unsInt count, sizeOfText, letNumb = 0;
    unsInt cur_st = 0, i;
    for(i = 0; (history[cur_st][i] = getwc(fin)) != WEOF; i++)
    {
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
    for(unsigned int chng_it = 0; chng_it < i - 1; chng_it++)
    {
        wchInfo tempOne;
        int t;
        for(unsigned int counter = chng_it + 1; counter < sizeOfText; counter++)
        {
            if(freqOfSymb[chng_it].count < freqOfSymb[counter].count)
                t = counter;
        }
        if(chng_it < t)
        {
            tempOne.chr = freqOfSymb[t].chr;
            tempOne.count = freqOfSymb[t].count;
            freqOfSymb[t].chr = freqOfSymb[chng_it].chr;
            freqOfSymb[t].count = freqOfSymb[chng_it].count;
            freqOfSymb[chng_it].chr = tempOne.chr;
            freqOfSymb[chng_it].count = tempOne.count; 
        }
        letNumb += freqOfSymb[chng_it].count;
    }

    usrInterface(&cur_st, freqOfSymb, history, sizeOfText, letNumb, outFName);

    fclose(fin);
    
}

bool isLetter(wchar_t symbol)
{
    if(symbol >= L'А' && symbol <= L'Я' &&
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

void usrInterface(unsInt * current_step, wchInfo cryptAlp[], wchar_t actTxt[][textSize], unsInt realTextSize, unsInt lettNumb, char outFName[])
{
    unsigned short usrAns = 12;
    while(usrAns != 0)
    {
        while(usrAns > 7)
        {
            printf("1 | Вывод предполагаемых замен\n");
            printf("2 | Отменить последние изменения\n");
            printf("3 | Вывод слов по длине\n");
            printf("4 | Вывод слов по дешифровке\n");
            printf("5 | Вывод текста с заменами\n");
            printf("6 | Автозамена букв\n");
            printf("7 | Автозамена букв\n");
            printf("0 | Запись рез-ов и выход\n");
        }

        switch(usrAns)
        {
            case 1:
                outPossReplace(cryptAlp, lettNumb);
                break;
            case 2:
                Undo(current_step);
                break;
            case 3:
                outWordsByLen();
                break;
            case 4:
                outByUncrypt();
                break;
            case 5:
                outActTextCond(actTxt, current_step, realTextSize);
                break;
            case 6:
                autoDecrypt(actTxt, current_step, realTextSize, cryptAlp);
                break;
            case 7:
                manualReplace();
                break;
            case 0:
                outToFile(outFName, actTxt, current_step, realTextSize);
                break;
            default:
                printf("wrong number!\n");
                break;
        }
    }
}

void outPossReplace(wchInfo cryptAlp[], unsInt lettNumb)
{
    printf("possible replacements : \n");
    for(int i = 0; i < 33; i++)
    {
        wprintf(L"%lc | %Ld >===> %lc\n", cryptAlp[i].chr, (long double)cryptAlp[i].count / lettNumb, sortedAlph[i]);
    }
}

void Undo(unsInt * current_step)
{
    if(*current_step == 0)
        *current_step = steps_in_memory - 1;
    else
        *current_step -= 1;
}

void outWordsByLen()
{

}

void outByUncrypt()
{

}

void manualReplace()
{

}

void outActTextCond(wchar_t actTxt[][textSize], unsInt * current_step, unsInt realTextSize)
{
    for(unsInt i = 0; i < realTextSize; i++)
        wprintf(L"%lc", actTxt[*current_step][i]);
    
    printf("\n");
}

void autoDecrypt(wchar_t actTxt[][textSize], unsInt * current_step, unsInt realTextSize, wchInfo cryptAlp[])
{
    unsInt ftrStep = (*current_step + 1) % steps_in_memory;
    for(unsInt t = 0; t < realTextSize; t++)
    {
        actTxt[ftrStep][t] = actTxt[*current_step][t];
    }
    for(unsInt i = 0; i < 33 && cryptAlp[i].count > 0; i++)
    {
        for(unsInt t = 0; t < realTextSize; t++)
        {
            if(actTxt[*current_step][t] == cryptAlp[i].chr)
                actTxt[ftrStep][t] = sortedAlph[i];
        }
    }
}

void outToFile(char outFName[], wchar_t actTxt[][textSize], unsInt * current_step, unsInt realTextSize)
{
    FILE *fout = fopen(outFName, "w");
    for(unsInt i = 0; i < realTextSize; i++)
        fwprintf(fout, L"%lc", actTxt[*current_step][i]);
    fclose(fout);
}