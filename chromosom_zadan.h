#ifndef CHROMOSOM_ZADAN_H
#define CHROMOSOM_ZADAN_H
#include <list>
#include "zadanie.h"
#include "zespol.h"

extern unsigned short liczba_zadan;
extern zadanie *lista_zadan;

class chromosom_zadan
{ 
private:
     short *geny;
      
     friend class przystosowanie;
     friend class harmonogram_txt;
public:
     chromosom_zadan();
     chromosom_zadan(chromosom_zadan &wzor_chr);
     ~chromosom_zadan();
     chromosom_zadan & operator=(const chromosom_zadan &wzor_chr);
     void LosujChrZadan();
     void MutacjaZamianaGenow(const unsigned short prawdop_mutacji);
     void MutacjaZmianaPozycji(const unsigned short prawdop_mutacji);
};
#endif
