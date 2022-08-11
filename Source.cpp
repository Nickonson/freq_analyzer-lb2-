#include <locale.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#define steps_in_memory 30
#define textSize 1000

using namespace std;

const char* sortedAlphabet = "оеаинтсрвлкмдпуяыьгзбчйхжшюцщэфъ";

char history[steps_in_memory][textSize];

bool isLetter(char symbol);

int letterNumb(char symbol);

int main()
{
    
    
    FILE *fin = fopen("oib/lab 2/finut.txt", "r");
    FILE *fout = fopen("oib/lab 2/foutput.txt", "w");
    
    unsigned int freqOfSymb[33];
    for(int i = 0; i < 33; i++)
        freqOfSymb[i] = 0;

    unsigned int count, sizeOfText;
    unsigned int cur_st = 0, i = 0;
    for(; (history[cur_st][i] = getc(fin)) != EOF; i++)
    sizeOfText = i;
    history[cur_st][i] = '\0';
    
    fseek(fin, 0, SEEK_SET);
    
    for(unsigned int counter = 0; counter < i; counter++)
    {
        if(isLetter(history[cur_st][counter]))
        {
            freqOfSymb[letterNumb(history[cur_st][counter])]++;
        }
    }

    fclose(fin);
    fclose(fout);
}

bool isLetter(char symbol)
{
    if(symbol >= 'а' && symbol <= 'я' &&
        symbol >= 'А' && symbol <= 'Я')
        return true;
    return false;
}

int letterNumb(char symbol)
{
    if(symbol >= 'А' && symbol <= 'Я')
        symbol -= 'А' + 'а';
    return symbol;
}