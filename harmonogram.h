#ifndef HARMONOGRAM_H
#define HARMONOGRAM_H
#include "chromosom_zasobow.h"
#include "chromosom_zadan.h"

class harmonogram
{
private:
protected:
     int czas_wykonania_projektu;
     std::string nazwa;
public:
     harmonogram()
     {
          czas_wykonania_projektu = 0;
          nazwa = "";
     }
     virtual bool TworzHarmonogram(chromosom_zadan &chr_zadania,
                                   chromosom_zasobow &chr_zasobu,
                                   const char *nazwa_harmonogramu,
                                   int czas_wykonania) = 0;
};
#endif
