#ifndef PRZYSTOSOWANIE_H
#define PRZYSTOSOWANIE_H
#include "zadanie.h"
#include "zespol.h"
#include "chromosom_zasobow.h"
#include "chromosom_zadan.h"

extern unsigned short liczba_zadan,
                      liczba_zespolow;
extern zadanie *lista_zadan;
extern zespol *lista_zespolow;

class przystosowanie
{
private:
     int wartosc_f_celu;   
public:
     int PobierzWartoscFCelu()
     {
         return wartosc_f_celu;
     }
     void LiczFunkcjeCelu(chromosom_zadan &chr_zadania, chromosom_zasobow &chr_zasoby);
};
#endif
