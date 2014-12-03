#include <iostream>
#include <time.h>
#include "zadanie.h"
#include "zespol.h"
#include "chromosom_zadan.h"
#include "chromosom_zasobow.h"
#include "operacje_ewolucyjne.h"
#include "przystosowanie.h"
#include "harmonogram.h"
#include "harmonogram_txt.h"
#include "funkcje_selekcji.h"
using namespace std;

// Dane początkowe
unsigned short liczba_zadan,
               liczba_zespolow;
zadanie *lista_zadan;
zespol *lista_zespolow;
                     
bool TworzLosowyProblem(unsigned short maks_roboczodni,
                        unsigned short min_roboczodni,
                        unsigned short maks_zespol);
void WczytajMalyPrzyklad();
void WczytajDuzyPrzyklad();
//------------------------------------------------------------------------------                     
int main()
{         
    // Parametry algorytmu ewolucyjnego
    unsigned short liczba_chromosomow = 6;
    unsigned short liczba_iteracji = 800;
    // (im większa liczba, tym mniejsze prawdopodobieństwo zajścia mutacji)
    unsigned short prawdop_mutacji_zadania = 20;
    unsigned short prawdop_mutacji_zespoly = 50;   
            
    srand(static_cast<unsigned int>(time(NULL)));
    
    enum czynnosc{ wczytaj_maly_przyklad,
                   wczytaj_duzy_przyklad,
                   losuj_maly_problem,
                   losuj_duzy_problem };
    short nr_czynnosci = 0;
                   
    cout << "Prosze wybrac numer czynnosci:\n"
         << "---------------------------------------------------------------\n"
         << "1. Wczytanie przykladu malego projektu (10 zadan, 2 zespoly)\n" 
         << "2. Wczytanie przykladu duzego projektu (50 zadan, 5 zespolow)\n"
         << "3. Wylosowanie przykladu malego projektu (10 zadan, 2 zespoly)\n"
         << "4. Wylosowanie przykladu duzego projektu (50 zadan, 5 zespolow)\n"
         << "---------------------------------------------------------------\n";                       
    do
    { 
       cin >> nr_czynnosci;
    }while(!(nr_czynnosci >= 1 && nr_czynnosci <= 4));
    
    switch(czynnosc wybor_czynnosci = czynnosc(nr_czynnosci-1))
    {   
        case wczytaj_maly_przyklad:
        { 
            WczytajMalyPrzyklad();
            break;
        }
        case wczytaj_duzy_przyklad:
        { 
            WczytajDuzyPrzyklad();
            break;
        }
        case losuj_maly_problem:
        { 
            liczba_zadan = 10;
            liczba_zespolow = 2; 
             
            lista_zadan = new zadanie[liczba_zadan];
            lista_zespolow = new zespol[liczba_zespolow];   
            
            if(TworzLosowyProblem(10, 1, 10) == false)
            {
                delete [] lista_zadan;
                delete [] lista_zespolow;
                
                cout << "Blad przy tworzeniu losowych zadan!\n";
                system("pause");
                return 0;
            }
            break;
        }
        case losuj_duzy_problem:
        { 
            liczba_zadan = 50;
            liczba_zespolow = 5; 
             
            lista_zadan = new zadanie[liczba_zadan];
            lista_zespolow = new zespol[liczba_zespolow];   
            
            if(TworzLosowyProblem(10, 1, 10) == false)
            {
                delete [] lista_zadan;
                delete [] lista_zespolow;
                
                cout << "Blad przy tworzeniu losowych zadan!\n";
                system("pause");
                return 0;
            }
            break;
        }
    }
    
    clock_t czas_dzialania_programu = clock();
    
    chromosom_zadan *chr_zadania = new chromosom_zadan[liczba_chromosomow];
    chromosom_zasobow *chr_zasoby = new chromosom_zasobow[liczba_chromosomow];
    short nr_najl_chr_zadan;
    short nr_najl_chr_zasobow;
    
    for(short nr_chr = 0; nr_chr < liczba_chromosomow; nr_chr++)
    {
         chr_zadania[nr_chr].LosujChrZadan();
         chr_zasoby[nr_chr].LosujChrZasobow();        
    }
    
    przystosowanie *przyst_chromosomow = new przystosowanie[liczba_chromosomow*liczba_chromosomow];
    enum rodzaj_krzyzowania_zasobow{ jednopunktowe, dwupunktowe, jednorodne };
    enum rodzaj_mutacji_zadan{ zamiana_genow, zmiana_pozycji };
    enum rodzaj_selekcji{ kolo_ruletki, rankingowa };
    
    rodzaj_krzyzowania_zasobow jakie_krzyzowanie_zasobow = jednorodne;
    rodzaj_mutacji_zadan       jaka_mutacja_zadan = zmiana_pozycji;
    rodzaj_selekcji            jaka_selekcja = rankingowa;
    
    enum typ_harmonogramu{ plik_tekstowy, arkusz_excel };
    typ_harmonogramu jaki_harmonogram = plik_tekstowy;
    
    harmonogram_txt harmonogram_pocz_txt;
    harmonogram *harmonogram_poczatkowy;
    
    switch(jaki_harmonogram)
    {
         case plik_tekstowy:
         {
              harmonogram_poczatkowy = &harmonogram_pocz_txt;
              break;
         }          
         case arkusz_excel:
         {
              
              break;
         }                
    }
    harmonogram_poczatkowy->UstawNazwe("harmonogram_poczatkowy");
    BadajPrzystosowanie(przyst_chromosomow, chr_zadania, chr_zasoby, liczba_chromosomow);
    ZnajdzNajlepszaPareChr(przyst_chromosomow,
                           liczba_chromosomow,
                           nr_najl_chr_zadan,
                           nr_najl_chr_zasobow);
    harmonogram_poczatkowy->UstawCzasRealizacji(przyst_chromosomow[nr_najl_chr_zadan*liczba_chromosomow
                                                + nr_najl_chr_zasobow].PobierzWartoscFCelu()); 
    cout << "\nPo zastosowaniu algorytmu. . .\n"
         << "\tNajlepszy czas ukonczenia projektu: "
         << harmonogram_poczatkowy->PobierzCzasRealizacji()
         << endl;
    if(harmonogram_poczatkowy->TworzHarmonogram(chr_zadania[nr_najl_chr_zadan], 
                                                chr_zasoby[nr_najl_chr_zasobow]))
        cout << "\tPomyslnie wyeksportowano harmonogram do pliku.\n";
    else
        cout << "\tBlad przy eksporcie harmonogramu do pliku!\n";
        
    delete harmonogram_poczatkowy; 

    // Algorytm ewolucyjny
    for(int generacja = 1; generacja <= liczba_iteracji; generacja++)
    {            
        KrzyzujChromosomyZasobow(chr_zasoby,
                                 liczba_chromosomow,
                                 jakie_krzyzowanie_zasobow);
        MutujChromosomyZadan(chr_zadania,
                             liczba_chromosomow,
                             jaka_mutacja_zadan,
                             prawdop_mutacji_zadania);                  
        MutujChromosomyZasobow(chr_zasoby,
                               liczba_chromosomow,
                               prawdop_mutacji_zespoly);
        BadajPrzystosowanie(przyst_chromosomow,
                            chr_zadania,
                            chr_zasoby,
                            liczba_chromosomow);
        DokonajSelekcji(przyst_chromosomow,
                        chr_zadania,
                        chr_zasoby,
                        liczba_chromosomow,
                        jaka_selekcja);
    }
    
    harmonogram_txt harmonogram_konc_txt;
    harmonogram *harmonogram_koncowy;
    
    switch(jaki_harmonogram)
    {
         case plik_tekstowy:
         {
              harmonogram_koncowy = &harmonogram_konc_txt;
              break;
         }          
         case arkusz_excel:
         {
              break;
         }                
    }
    harmonogram_koncowy->UstawNazwe("harmonogram_koncowy");
    BadajPrzystosowanie(przyst_chromosomow, chr_zadania, chr_zasoby, liczba_chromosomow);
    ZnajdzNajlepszaPareChr(przyst_chromosomow,
                           liczba_chromosomow,
                           nr_najl_chr_zadan,
                           nr_najl_chr_zasobow);
    harmonogram_koncowy->UstawCzasRealizacji(przyst_chromosomow[nr_najl_chr_zadan*liczba_chromosomow
                                             + nr_najl_chr_zasobow].PobierzWartoscFCelu()); 
    cout << "\nPo zastosowaniu algorytmu. . .\n"
         << "\tNajlepszy czas ukonczenia projektu: "
         << harmonogram_koncowy->PobierzCzasRealizacji()
         << endl;
    if(harmonogram_koncowy->TworzHarmonogram(chr_zadania[nr_najl_chr_zadan], 
                                             chr_zasoby[nr_najl_chr_zasobow]))
        cout << "\tPomyslnie wyeksportowano harmonogram do pliku.\n";
    else
        cout << "\tBlad przy eksporcie harmonogramu do pliku!\n";
        
    delete harmonogram_koncowy;
    
    czas_dzialania_programu = clock() - czas_dzialania_programu;
    cout << "\nCzas obliczen algorytmu: "
         << ((float)czas_dzialania_programu)/CLOCKS_PER_SEC << " s\n\n"; 

    delete [] lista_zadan;
    delete [] lista_zespolow;
    delete [] chr_zadania;
    delete [] chr_zasoby;
    system("pause");
    return 0;
}
//------------------------------------------------------------------------------
bool TworzLosowyProblem(unsigned short maks_roboczodni,
                        unsigned short min_roboczodni,
                        unsigned short maks_zespol)
{
     if(maks_roboczodni < min_roboczodni
        || liczba_zadan < 5
        || liczba_zespolow < 2
        || maks_zespol < 1)
     {
         return false;
     }

     for(unsigned short nr_zadania = 0; nr_zadania < liczba_zadan; nr_zadania++)
     {
         lista_zadan[nr_zadania].LosujZadanie(lista_zadan, maks_roboczodni, min_roboczodni);
     } 
     
     for(unsigned short nr_zespolu = 0; nr_zespolu < liczba_zespolow; nr_zespolu++)
     {
         lista_zespolow[nr_zespolu].LosujZespol(maks_zespol);     
     }
    
     return true; 
}
