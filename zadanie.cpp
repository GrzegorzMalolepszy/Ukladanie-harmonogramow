#include "zadanie.h"
#include <iostream>
using namespace std;

short zadanie::liczba_obiektow = 0;

zadanie::zadanie()
{
    id = liczba_obiektow++;
    nazwa = "WP";
    if(liczba_obiektow < 10)
         nazwa += char(liczba_obiektow + 48);
    else if(liczba_obiektow < 100)
    {
         // dziesiątki
         nazwa += char(static_cast<unsigned short>(liczba_obiektow/10) + 48);
         // jedności
         nazwa += char(liczba_obiektow%10 + 48);
    }
    else if(liczba_obiektow < 1000)
    {
         unsigned short i = static_cast<unsigned short>(liczba_obiektow/100);
         // setki
         nazwa += char(i + 48);
         // dziesiątki
         nazwa += char(static_cast<unsigned short>((liczba_obiektow-i*100)/10) + 48);
         // jedności
         nazwa += char(liczba_obiektow%10 + 48);
    }
}
//------------------------------------------------------------------------------
zadanie::~zadanie()
{
    liczba_obiektow--;
}
//------------------------------------------------------------------------------
void zadanie::LosujZadanie(zadanie lista_zadan[], unsigned short maks_roboczodni, unsigned short min_roboczodni)
{
    do
    {
        roboczodni = rand() % maks_roboczodni + 1;
    }while(roboczodni < min_roboczodni);
    
    nr_zespolu = rand() % liczba_zespolow;
    /* Losowanie zadania poprzedzającego;
       1/10 zadań może nie mieć żadnych poprzedników */
    if(id && rand() % 10)
    {
        unsigned short nr_bezp_poprzendika = rand() % id;
        poprzedniki.push_back(&lista_zadan[nr_bezp_poprzendika]);
        
        for(list<zadanie *>::iterator it=lista_zadan[nr_bezp_poprzendika].poprzedniki.begin();
            it != lista_zadan[nr_bezp_poprzendika].poprzedniki.end(); ++it)
        {
            poprzedniki.push_back(*it);
        }
    }
}
//------------------------------------------------------------------------------
bool zadanie::CzyToPoprzednik(short nr_zadania_poprzedzajacego)
{
    for(list<zadanie *>::iterator it = poprzedniki.begin();
        it != poprzedniki.end(); ++it)
    {
        if((*(*it)).id == nr_zadania_poprzedzajacego)
             return true;
    }
    return false;
}
//------------------------------------------------------------------------------
std::string zadanie::ListaPoprzednikowJakoString()
{
    string lista;
    for(list<zadanie *>::iterator it = poprzedniki.begin();
        it != poprzedniki.end();
        ++it)
    {
         lista += ((*(*it)).nazwa + "\t");
    } 
    return lista;
}
