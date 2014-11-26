#include "chromosom_zadan.h"
using namespace std;

chromosom_zadan::chromosom_zadan()
{
     geny = new short[liczba_zadan];
}
//------------------------------------------------------------------------------
chromosom_zadan::chromosom_zadan(chromosom_zadan &wzor_chr)
{
     geny = new short[liczba_zadan];   
     for(short nr_genu = 0; nr_genu < liczba_zadan; nr_genu++)
          geny[nr_genu] = wzor_chr.geny[nr_genu];                                             
}
//------------------------------------------------------------------------------
chromosom_zadan::~chromosom_zadan()
{
     delete [] geny;
}
//------------------------------------------------------------------------------
chromosom_zadan & chromosom_zadan::operator=(const chromosom_zadan &wzor_chr)
{
     for(short nr_genu = 0; nr_genu < liczba_zadan; nr_genu++)
          geny[nr_genu] = wzor_chr.geny[nr_genu];
}
//------------------------------------------------------------------------------
void chromosom_zadan::LosujChrZadan()
{
     list<zadanie *>::iterator it;
     unsigned short nr_zad, 
                    poz1, poz2,
                    proby_zamiany_kolejnosci = 0,
                    kopia_genu;   
     
     // Przypisanie początkowej, niezmienionej kolejności zadań   
     for(nr_zad = 0; nr_zad < liczba_zadan; nr_zad++)
          geny[nr_zad] = lista_zadan[nr_zad].PobierzId();
     
     // Mieszanie kolejności zadań (genów chromosomu)
     do
     {
         poz1 = rand() % liczba_zadan;
         do
         {
             poz2 = rand() % liczba_zadan;
         }while(poz1 == poz2);
         kopia_genu = geny[poz1]; 
         geny[poz1] = geny[poz2];
         geny[poz2] = kopia_genu;
         
         /* Sprawdzanie poprawności kolejności wszystkich zadań do momentu znalezienia
            choć jednej niezgodności */
         for(nr_zad = 0; nr_zad < liczba_zadan; nr_zad++)
         {
              kopia_genu = geny[nr_zad];
              if(lista_zadan[kopia_genu].CzyMaPoprzednikow() == false)
                  continue;
              // Sprawdzanie czy któryś poprzednik stoi w kolejce za zadaniem zamiast przed nim
              for(unsigned short z = nr_zad+1; z < liczba_zadan; z++)
              {
                  if(lista_zadan[kopia_genu].CzyToPoprzednik(geny[z]))
                  {
                       kopia_genu = geny[poz1]; 
                       geny[poz1] = geny[poz2];
                       geny[poz2] = kopia_genu;
                       goto po_napr_bledu;
                  }
              }
         }
         po_napr_bledu:
         proby_zamiany_kolejnosci++;
     }while(proby_zamiany_kolejnosci < liczba_zadan*2);
}
//------------------------------------------------------------------------------
void chromosom_zadan::MutacjaZamianaGenow(const unsigned short prawdop_mutacji)
{
     unsigned short licznik_bledow;
     bool blad;
     unsigned short nowa_pozycja;     
     unsigned short kopia_genu;

     // Przedział od pierwszego do przedostatniego zadania w kolejce
     for(unsigned short nr_genu = 0; nr_genu < liczba_zadan-1; nr_genu++)
     {
         if(rand() % prawdop_mutacji == 0)
         {
              licznik_bledow = 0;
              
              do
              {
                  nowa_pozycja = rand() % (liczba_zadan-nr_genu) + nr_genu;
                  
                  // Zamiana zadań miejscami
                  kopia_genu = geny[nr_genu];
                  geny[nr_genu] = geny[nowa_pozycja];
                  geny[nowa_pozycja] = kopia_genu;
                  
                  // Sprawdzanie czy po zadaniu znajdzie się jakikolwiek poprzednik
                  blad = false;
                  for(short poz = nr_genu+1; poz <= nowa_pozycja; poz++)
                  {
                     if(lista_zadan[geny[nr_genu]].CzyToPoprzednik(geny[poz]))
                     {
                         licznik_bledow++;
                         // Powtórna zamiana zadań miejscami w przypadku konfliktu 
                         kopia_genu = geny[nr_genu];
                         geny[nr_genu] = geny[nowa_pozycja];
                         geny[nowa_pozycja] = kopia_genu;  
                         blad = true;
                         break;                                         
                     }
                  }
                  
                  if(blad)
                     continue; 
                  
                  // Sprawdzanie czy zadanie nie jest poprzednikiem zadań stojących wcześniej
                  for(short poz = nowa_pozycja-1; poz >= nr_genu; poz--)
                  {
                     if(lista_zadan[geny[poz]].CzyToPoprzednik(geny[nowa_pozycja]))
                     {
                         licznik_bledow++;
                         kopia_genu = geny[nr_genu];
                         geny[nr_genu] = geny[nowa_pozycja];
                         geny[nowa_pozycja] = kopia_genu;
                         blad = true;
                         break;
                     }
                  }
              }while(blad && licznik_bledow < 10);
         }
    }
}
//------------------------------------------------------------------------------
void chromosom_zadan::MutacjaZmianaPozycji(const unsigned short prawdop_mutacji)
{
     short licznik_bledow;
     bool blad;
     unsigned short nowa_pozycja;     
     unsigned short kopia_genu;
     
     for(unsigned short nr_genu = 1; nr_genu < liczba_zadan; nr_genu++)
     {
         if(rand() % prawdop_mutacji == 0)
         {
              licznik_bledow = 0;
              
              do
              {
                  // Losowanie jednej ze wcześniejszych pozycji dla genu
                  nowa_pozycja = rand() % nr_genu;

                  // Sprawdzanie czy po zadaniu znajdzie się jakikolwiek poprzednik
                  blad = false;
                  
                  if(lista_zadan[geny[nr_genu]].CzyMaPoprzednikow() == false)
                         continue;
                         
                  for(short poz = nowa_pozycja; poz <= nr_genu; poz++)
                  {
                     if(lista_zadan[geny[nr_genu]].CzyToPoprzednik(geny[poz]))
                     {
                         licznik_bledow++;
                         blad = true;
                         break;                                                     
                     }
                  }
              }while(blad && licznik_bledow<3);
              
              if(licznik_bledow < 3)
              {                             
                  kopia_genu = geny[nr_genu];
                  
                  for(short poz = nr_genu-1; poz >= nowa_pozycja; poz--)
                  {
                      geny[poz+1] = geny[poz];
                      if(poz==0)
                           break;
                  }
                  
                  geny[nowa_pozycja] = kopia_genu;
              }
         }
     }
}
