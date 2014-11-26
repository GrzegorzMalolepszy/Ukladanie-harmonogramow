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
bool TworzHarmonogramNajlRozwiazania(przystosowanie przyst_chromosomow[],
                                     chromosom_zadan chr_zadania[],
                                     chromosom_zasobow chr_zasoby[],
                                     unsigned short liczba_chromosomow,
                                     const char *nazwa_harmonogramu,
                                     harmonogram *wsk_harmonogramu);
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
    
    harmonogram_txt harmonogram_pocz_txt;
    harmonogram *wsk_harmonogramu;
    wsk_harmonogramu = &harmonogram_pocz_txt;
    
    cout << "\nPrzed zastosowaniem algorytmu. . .\n";
    if(TworzHarmonogramNajlRozwiazania(przyst_chromosomow,
                                       chr_zadania,
                                       chr_zasoby,
                                       liczba_chromosomow,
                                       "harmonogram_poczatkowy",
                                       wsk_harmonogramu))
        cout << "\tPomyslnie wyeksportowano harmonogram do pliku.\n";
    else
        cout << "\tBlad przy eksporcie harmonogramu do pliku!\n";

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
    wsk_harmonogramu = &harmonogram_konc_txt;
    
    cout << "\nPo zastosowaniu algorytmu. . .\n";
    if(TworzHarmonogramNajlRozwiazania(przyst_chromosomow,
                                       chr_zadania,
                                       chr_zasoby,
                                       liczba_chromosomow,
                                       "harmonogram_koncowy",
                                       wsk_harmonogramu))
        cout << "\tPomyslnie wyeksportowano harmonogram do pliku.\n";
    else
        cout << "\tBlad przy eksporcie harmonogramu do pliku!\n";
    
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

     for(unsigned short nr_zadania=0; nr_zadania<liczba_zadan; nr_zadania++)
     {
         lista_zadan[nr_zadania].LosujZadanie(lista_zadan, maks_roboczodni, min_roboczodni);
     } 
     
     for(unsigned short nr_zespolu=0; nr_zespolu<liczba_zespolow; nr_zespolu++)
     {
         lista_zespolow[nr_zespolu].LosujZespol(maks_zespol);     
     }
    
     return true; 
}
//------------------------------------------------------------------------------
bool TworzHarmonogramNajlRozwiazania(przystosowanie przyst_chromosomow[],
                                     chromosom_zadan chr_zadania[],
                                     chromosom_zasobow chr_zasoby[],
                                     unsigned short liczba_chromosomow,
                                     const char *nazwa_harmonogramu,
                                     harmonogram *wsk_harmonogramu)
{
    BadajPrzystosowanie(przyst_chromosomow, chr_zadania, chr_zasoby, liczba_chromosomow);

    short nr_najl_chr_zadan,
          nr_najl_chr_zasobow;
          
    ZnajdzNajlepszaPareChr(przyst_chromosomow,
                           liczba_chromosomow,
                           nr_najl_chr_zadan,
                           nr_najl_chr_zasobow);
    cout << "\tNajlepszy czas ukonczenia projektu: "
         <<  przyst_chromosomow[nr_najl_chr_zadan*liczba_chromosomow
                                + nr_najl_chr_zasobow].PobierzWartoscFCelu()
         << endl;

    return wsk_harmonogramu->TworzHarmonogram(chr_zadania[nr_najl_chr_zadan],
                                              chr_zasoby[nr_najl_chr_zasobow],
                                              nazwa_harmonogramu,
                                              przyst_chromosomow[nr_najl_chr_zadan*liczba_chromosomow
                                                                 + nr_najl_chr_zasobow].PobierzWartoscFCelu());
}
