#include "zespol.h"
#include <iostream>

short zespol::liczba_obiektow = 0;
unsigned short zespol::najwiecej_w_zespole = 0;

zespol::zespol()
{
    id = liczba_obiektow++;
    
    // Nadawanie domyślnej nazwy zespołu
    nazwa = "Zespol";
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
zespol::~zespol()
{
    liczba_obiektow--;     
}
//------------------------------------------------------------------------------
void zespol::LosujZespol(unsigned short maks_zespol, unsigned short min_zespol)
{
    do
    {
        liczba_pracownikow = rand() % maks_zespol + 1;
    }while(liczba_pracownikow < min_zespol);
    if(liczba_pracownikow > najwiecej_w_zespole)
        najwiecej_w_zespole = liczba_pracownikow;
}
