#ifndef FUNKCJE_SELEKCJI_H
#define FUNKCJE_SELEKCJI_H
#include "przystosowanie.h"
#include "chromosom_zadan.h"
#include "chromosom_zasobow.h"

void ZnajdzNajlepszaPareChr(przystosowanie przyst_chromosomow[],
                            unsigned short liczba_chromosomow,
                            short &nr_najlepszego_chr_zadan,
                            short &nr_najlepszego_chr_zasobow);  
void SelekcjaKoloRuletki(przystosowanie przyst_chromosomow[],
                         chromosom_zadan chr_zadania[],
                         chromosom_zasobow chr_zasoby[],
                         unsigned short liczba_chromosomow);  
void SelekcjaRankingowa(przystosowanie przyst_chromosomow[],
                        chromosom_zadan chr_zadania[],
                        chromosom_zasobow chr_zasoby[],
                        unsigned short liczba_chromosomow); 
unsigned int LiczWartosciFPrzystosowania(przystosowanie przyst_chromosomow[],
                                         unsigned short liczba_chromosomow,
                                         short wartosci_f_przystosowania[]);
#endif
