#include "operacje_ewolucyjne.h"
#include "funkcje_selekcji.h"
#include <iostream>
#include <cstdlib>

void KrzyzujChromosomyZasobow(chromosom_zasobow chr_zasoby[],
                              unsigned short liczba_chromosomow,
                              int rodzaj_krzyzowania)
{    
     unsigned short nr_rodzica1,
                    nr_rodzica2;
     chromosom_zasobow *kopia_chr_zasoby = new chromosom_zasobow[liczba_chromosomow];
     
     for(short nr_chr = 0; nr_chr < liczba_chromosomow; nr_chr++)
         kopia_chr_zasoby[nr_chr] = chr_zasoby[nr_chr];
                    
     for(short nr_krzyzowania = 0;
         nr_krzyzowania < liczba_chromosomow;
         nr_krzyzowania++)
     {
         // Losowanie pary chromosomów do krzyżowania
         nr_rodzica1 = rand() % liczba_chromosomow;
         do
         {
             nr_rodzica2 = rand() % liczba_chromosomow;
         }while(nr_rodzica1 == nr_rodzica2);
         
         // Krzyżowanie i przypisanie jednego z potomków do nowej populacji
         switch(rodzaj_krzyzowania)
         {
             case 0:
             {
                 chr_zasoby[nr_krzyzowania].KrzyzowanieJednopunktowe(kopia_chr_zasoby[nr_rodzica1],
                                                                     kopia_chr_zasoby[nr_rodzica2]);
                 break;
             }
             case 1:
             {
                 chr_zasoby[nr_krzyzowania].KrzyzowanieDwupunktowe(kopia_chr_zasoby[nr_rodzica1],
                                                                   kopia_chr_zasoby[nr_rodzica2]);
                 break;
             }
             case 2:
             {
                 chr_zasoby[nr_krzyzowania].KrzyzowanieJednorodne(kopia_chr_zasoby[nr_rodzica1],
                                                                  kopia_chr_zasoby[nr_rodzica2]);
                 break;
             }
         }
     }
     
     delete[] kopia_chr_zasoby;
}
//------------------------------------------------------------------------------
void MutujChromosomyZadan(chromosom_zadan chr_zadania[],
                          unsigned short liczba_chromosomow,
                          int rodzaj_mutacji,
                          const unsigned short prawdop_mutacji)
{
     for(short nr_chr = 0; nr_chr < liczba_chromosomow; nr_chr++)
     {
         switch(rodzaj_mutacji)
         {
             case 0:
             {
                  chr_zadania[nr_chr].MutacjaZamianaGenow(prawdop_mutacji);
                  break;
             }
             case 1:
             {
                  chr_zadania[nr_chr].MutacjaZmianaPozycji(prawdop_mutacji);
                  break;
             }
         }
     }
}
//------------------------------------------------------------------------------
void MutujChromosomyZasobow(chromosom_zasobow chr_zasoby[],
                            unsigned short liczba_chromosomow,
                            const unsigned short prawdop_mutacji)
{
     for(short nr_chr = 0; nr_chr < liczba_chromosomow; nr_chr++)
     {
         chr_zasoby[nr_chr].MutacjaZmianaWartosci(prawdop_mutacji);
     }
}
//------------------------------------------------------------------------------
void BadajPrzystosowanie(przystosowanie przyst_chromosomow[],
                         chromosom_zadan chr_zadania[],
                         chromosom_zasobow chr_zasoby[],
                         unsigned short liczba_chromosomow)
{
     for(short nr_chr_zadan = 0; nr_chr_zadan < liczba_chromosomow; nr_chr_zadan++)
     {
         for(short nr_chr_zasobow = 0; nr_chr_zasobow < liczba_chromosomow; nr_chr_zasobow++)
         {
             przyst_chromosomow[nr_chr_zadan*liczba_chromosomow
                                + nr_chr_zasobow].LiczFunkcjeCelu(chr_zadania[nr_chr_zadan],
                                                                  chr_zasoby[nr_chr_zasobow]);
         }
     }
}
//------------------------------------------------------------------------------
void DokonajSelekcji(przystosowanie przyst_chromosomow[],
                     chromosom_zadan chr_zadania[],
                     chromosom_zasobow chr_zasoby[],
                     unsigned short liczba_chromosomow,
                     int rodzaj_selekcji)
{  
    switch(rodzaj_selekcji)
    {
        case 0:
        {
             SelekcjaKoloRuletki(przyst_chromosomow,
                                 chr_zadania,
                                 chr_zasoby,
                                 liczba_chromosomow);
             break;
        }
        case 1:
        {
             SelekcjaRankingowa(przyst_chromosomow,
                                chr_zadania,
                                chr_zasoby,
                                liczba_chromosomow);
             break;
        }    
    }
}
