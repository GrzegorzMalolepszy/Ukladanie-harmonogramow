#ifndef HARMONOGRAM_TXT_H
#define HARMONOGRAM_TXT_H
#include "harmonogram.h"
#include "zadanie.h"
#include "zespol.h"
#include "chromosom_zasobow.h"
#include "chromosom_zadan.h"

extern unsigned short liczba_zadan,
                      liczba_zespolow;
extern zadanie *lista_zadan;
extern zespol *lista_zespolow;

class harmonogram_txt : public harmonogram
{
private:
     bool ZapiszDoPliku(chromosom_zadan &chr_zadania,
                        chromosom_zasobow &chr_zasobu,
                        bool ** &wykres_zadan,
                        bool ** &wykres_zasobow);
public:
     bool TworzHarmonogram(chromosom_zadan &chr_zadania,
                           chromosom_zasobow &chr_zasobu);
};
#endif
