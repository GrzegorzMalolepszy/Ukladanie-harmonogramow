#ifndef OPERACJE_EWOLUCYJNE_H
#define OPERACJE_EWOLUCYJNE_H
#include "chromosom_zasobow.h"
#include "chromosom_zadan.h"
#include "przystosowanie.h"

extern unsigned short liczba_zadan;
extern zadanie *lista_zadan;
extern zespol *lista_zespolow;

void KrzyzujChromosomyZasobow(chromosom_zasobow chr_zasoby[],
                              unsigned short liczba_chromosomow,
                              int rodzaj_krzyzowania);
void MutujChromosomyZadan(chromosom_zadan chr_zadania[],
                          unsigned short liczba_chromosomow,
                          int rodzaj_mutacji,
                          const unsigned short prawdop_mutacji);
void MutujChromosomyZasobow(chromosom_zasobow chr_zasoby[],
                            unsigned short liczba_chromosomow,
                            const unsigned short prawdop_mutacji);
void BadajPrzystosowanie(przystosowanie przyst_chromosomow[],
                         chromosom_zadan chr_zadania[],
                         chromosom_zasobow chr_zasoby[],
                         unsigned short liczba_chromosomow);                                                             
void DokonajSelekcji(przystosowanie przyst_chromosomow[],
                     chromosom_zadan chr_zadania[],
                     chromosom_zasobow chr_zasoby[],
                     unsigned short liczba_chromosomow,
                     int rodzaj_selekcji); 
#endif
