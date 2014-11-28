#include "chromosom_zasobow.h"
#include <iostream>

chromosom_zasobow::chromosom_zasobow()
{
     geny = new short[liczba_zadan];      
}
//------------------------------------------------------------------------------
chromosom_zasobow::chromosom_zasobow(chromosom_zasobow &wzor_chr)
{
     geny = new short[liczba_zadan];   
     for(short nr_genu = 0; nr_genu < liczba_zadan; nr_genu++)
          geny[nr_genu] = wzor_chr.geny[nr_genu];                                             
}
//------------------------------------------------------------------------------
chromosom_zasobow::~chromosom_zasobow()
{
     delete [] geny;
}
//------------------------------------------------------------------------------
chromosom_zasobow & chromosom_zasobow::operator=(const chromosom_zasobow &wzor_chr)
{
     for(short nr_genu = 0; nr_genu < liczba_zadan; nr_genu++)
          geny[nr_genu] = wzor_chr.geny[nr_genu];
}
//------------------------------------------------------------------------------
void chromosom_zasobow::KopiujGeny(short kopia_genow[])
{
     for(short nr_genu = 0; nr_genu < liczba_zadan; nr_genu++)
          kopia_genow[nr_genu] = geny[nr_genu];
}
//------------------------------------------------------------------------------
void chromosom_zasobow::LosujChrZasobow()
{    
     unsigned short przydzial_zasobow;    
     
     for(short nr_zad = 0; nr_zad < liczba_zadan; nr_zad++)
     {
         do
         {
             przydzial_zasobow = rand() % (lista_zespolow[lista_zadan[nr_zad].PobierzNrZespolu()].PobierzLiczbePracownikow()) + 1;  
         }while(przydzial_zasobow > lista_zadan[nr_zad].PobierzRoboczodni()
                || (lista_zadan[nr_zad].PobierzRoboczodni() % przydzial_zasobow) != 0);
        
         geny[nr_zad] = przydzial_zasobow;
     }
}
//------------------------------------------------------------------------------
void chromosom_zasobow::KrzyzowanieJednopunktowe(chromosom_zasobow &rodzic1,
                                                 chromosom_zasobow &rodzic2)
{    
     unsigned short punkt_krzyzowania;
     short *geny_rodzica1 = new short[liczba_zadan],
           *geny_rodzica2 = new short[liczba_zadan];
     short *geny_potomka1 = new short[liczba_zadan],
           *geny_potomka2 = new short[liczba_zadan];
     
     rodzic1.KopiujGeny(geny_rodzica1);
     rodzic2.KopiujGeny(geny_rodzica2);
     
     punkt_krzyzowania = (rand() % (liczba_zadan-1)) + 1;
        
     // Kopiowanie niekrzyżowanych części
     for(short nr_genu = 0; nr_genu < punkt_krzyzowania; nr_genu++)
     {
         geny_potomka1[nr_genu] = geny_rodzica1[nr_genu];
         geny_potomka2[nr_genu] = geny_rodzica2[nr_genu];
     }
     
     // Krzyżowanie
     for(short nr_genu = punkt_krzyzowania; nr_genu < liczba_zadan; nr_genu++)
     {
         geny_potomka1[nr_genu] = geny_rodzica2[nr_genu];
         geny_potomka2[nr_genu] = geny_rodzica1[nr_genu];
     }
        
     // Losowanie jednego z dwóch potomków, który wejdzie do nowej populacji
     short nr_potomka = rand() % 2;
     for(short nr_genu = 0; nr_genu < liczba_zadan; nr_genu++)
     {
         if(nr_potomka)
              geny[nr_genu] = geny_potomka1[nr_genu];
         else
              geny[nr_genu] = geny_potomka2[nr_genu];
     }
     
     delete[] geny_rodzica1;
     delete[] geny_rodzica2;
     delete[] geny_potomka1;
     delete[] geny_potomka2;
}
//------------------------------------------------------------------------------
void chromosom_zasobow::KrzyzowanieDwupunktowe(chromosom_zasobow &rodzic1,
                                               chromosom_zasobow &rodzic2)
{    
     unsigned short punkt_krzyzowania1,
                    punkt_krzyzowania2;
     short *geny_rodzica1 = new short[liczba_zadan],
           *geny_rodzica2 = new short[liczba_zadan];
     short *geny_potomka1 = new short[liczba_zadan],
           *geny_potomka2 = new short[liczba_zadan];
     
     rodzic1.KopiujGeny(geny_rodzica1);
     rodzic2.KopiujGeny(geny_rodzica2);
          
     do
     {
         punkt_krzyzowania1 = (rand() % (liczba_zadan-1));
         punkt_krzyzowania2 = rand() % (liczba_zadan-1-punkt_krzyzowania1) + punkt_krzyzowania1 + 1;
     }while(punkt_krzyzowania1 == 0 && punkt_krzyzowania2 == liczba_zadan-1);
     
     // Krzyżowanie       
     for(short nr_genu = 0; nr_genu < liczba_zadan; nr_genu++)
     {
         if(nr_genu >= punkt_krzyzowania1 && nr_genu <= punkt_krzyzowania2)
         {
             geny_potomka1[nr_genu] = geny_rodzica2[nr_genu];
             geny_potomka2[nr_genu] = geny_rodzica1[nr_genu];
         }
         else
         {
             geny_potomka1[nr_genu] = geny_rodzica1[nr_genu];   
             geny_potomka2[nr_genu] = geny_rodzica2[nr_genu];
         }
     }
            
     // Losowanie jednego z dwóch potomków, który wejdzie do nowej populacji
     short nr_potomka = rand() % 2;
     for(short nr_genu = 0; nr_genu < liczba_zadan; nr_genu++)
     {
         if(nr_potomka)
              geny[nr_genu] = geny_potomka1[nr_genu];
         else
              geny[nr_genu] = geny_potomka2[nr_genu];
     }
     
     delete[] geny_rodzica1;
     delete[] geny_rodzica2;
     delete[] geny_potomka1;
     delete[] geny_potomka2;
}
//------------------------------------------------------------------------------
void chromosom_zasobow::KrzyzowanieJednorodne(chromosom_zasobow &rodzic1,
                                              chromosom_zasobow &rodzic2)
{    
     short *geny_rodzica1 = new short[liczba_zadan],
           *geny_rodzica2 = new short[liczba_zadan];  
     short *geny_potomka1 = new short[liczba_zadan],
           *geny_potomka2 = new short[liczba_zadan];

     rodzic1.KopiujGeny(geny_rodzica1);
     rodzic2.KopiujGeny(geny_rodzica2);

     for(short nr_genu = 0; nr_genu < liczba_zadan; nr_genu++)
     {
         if(rand() % 2)
         {
             geny_potomka1[nr_genu] = geny_rodzica2[nr_genu];
             geny_potomka2[nr_genu] = geny_rodzica1[nr_genu];
         }
         else
         {
             geny_potomka1[nr_genu] = geny_rodzica1[nr_genu];
             geny_potomka2[nr_genu] = geny_rodzica2[nr_genu];
         }
     }
     
     // Losowanie jednego z dwóch potomków, który wejdzie do nowej populacji
     short nr_potomka = rand() % 2;
     for(short nr_genu = 0; nr_genu < liczba_zadan; nr_genu++)
     {
         if(nr_potomka)
              geny[nr_genu] = geny_potomka1[nr_genu];
         else
              geny[nr_genu] = geny_potomka2[nr_genu];
     }
     
     delete[] geny_rodzica1;
     delete[] geny_rodzica2;
     delete[] geny_potomka1;
     delete[] geny_potomka2;
}
//------------------------------------------------------------------------------
void chromosom_zasobow::MutacjaZmianaWartosci(const unsigned short prawdop_mutacji)
{    
     bool blad;
     
     for(unsigned short nr_zadania = 0; nr_zadania < liczba_zadan; nr_zadania++)
     {
         if(rand() % prawdop_mutacji == 0
            && lista_zespolow[lista_zadan[nr_zadania].PobierzNrZespolu()].PobierzLiczbePracownikow() > 1)
         {
             /* Sprawdzanie czy jest możliwość mutacji, która dawałaby wykonalność
             zadania przez pracowników w pełnych dniach */
             blad = true;
             for(unsigned short pracownicy = 2;
                 pracownicy <= lista_zespolow[lista_zadan[nr_zadania].PobierzNrZespolu()].PobierzLiczbePracownikow();
                 pracownicy++)
             {
                 if(lista_zadan[nr_zadania].PobierzRoboczodni() % pracownicy == 0)
                 { blad = false;
                   break; }
             }
             
             if(!blad)
             {
                 unsigned short nowa_liczba_pracownikow;
                 do
                 {
                      nowa_liczba_pracownikow = rand() % lista_zespolow[lista_zadan[nr_zadania].PobierzNrZespolu()].PobierzLiczbePracownikow() + 1;
                 }while(geny[nr_zadania] == nowa_liczba_pracownikow
                        || nowa_liczba_pracownikow > lista_zadan[nr_zadania].PobierzRoboczodni()
                        || (lista_zadan[nr_zadania].PobierzRoboczodni() % nowa_liczba_pracownikow) != 0);
                       
                 geny[nr_zadania] = nowa_liczba_pracownikow;    
             }                 
         }
    }
}
