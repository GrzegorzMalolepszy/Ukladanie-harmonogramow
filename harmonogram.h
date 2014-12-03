#ifndef HARMONOGRAM_H
#define HARMONOGRAM_H
#include "chromosom_zasobow.h"
#include "chromosom_zadan.h"

class harmonogram
{
private:
protected:
     int czas_realizacji;
     std::string nazwa;
public:
     harmonogram()
     {
          czas_realizacji = 0;
          nazwa = "harmonogram";
     }
     void UstawCzasRealizacji(int czas_realizacji_projektu)
     {
          czas_realizacji = czas_realizacji_projektu;
     }
     void UstawNazwe(const char *nazwa_harmonogramu)
     {
          nazwa = nazwa_harmonogramu;
     }
     int PobierzCzasRealizacji()
     {
          return czas_realizacji;
     }
     virtual bool TworzHarmonogram(chromosom_zadan &chr_zadania,
                                   chromosom_zasobow &chr_zasobu) = 0;
};
#endif
