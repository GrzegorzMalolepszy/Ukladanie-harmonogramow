#include "funkcje_selekcji.h"
#include <iostream>
using namespace std;

void ZnajdzNajlepszaPareChr(przystosowanie przyst_chromosomow[],
                            unsigned short liczba_chromosomow,               
                            short &nr_najlepszego_chr_zadan,
                            short &nr_najlepszego_chr_zasobow)
{
     short nr_najlepszej_pary = 0;

     for(short nr_pary = 1;
         nr_pary < liczba_chromosomow*liczba_chromosomow;
         nr_pary++)
     {
         if(przyst_chromosomow[nr_pary].PobierzWartoscFCelu()
            < przyst_chromosomow[nr_najlepszej_pary].PobierzWartoscFCelu())
         {
             nr_najlepszej_pary = nr_pary;
         }
     }
     
     /* Szukanie numerów chromosomów z obu populajci odpowiadających 
        numerowi przystosowania dla pary */
     for(short nr_chr_zadan = 0; nr_chr_zadan < liczba_chromosomow; nr_chr_zadan++)
     {
         for(short nr_chr_zasobow = 0; nr_chr_zasobow < liczba_chromosomow; nr_chr_zasobow++)
         {
             if(nr_najlepszej_pary == nr_chr_zadan*liczba_chromosomow + nr_chr_zasobow)
             {
                 nr_najlepszego_chr_zadan = nr_chr_zadan;
                 nr_najlepszego_chr_zasobow = nr_chr_zasobow;
                 return;
             }
        }      
    }
}
//------------------------------------------------------------------------------
void SelekcjaKoloRuletki(przystosowanie przyst_chromosomow[],
                         chromosom_zadan chr_zadania[],
                         chromosom_zasobow chr_zasoby[],
                         unsigned short liczba_chromosomow)
{       
    short nr_najlepszego_chr_zadan,
          nr_najlepszego_chr_zasobow;
          
    ZnajdzNajlepszaPareChr(przyst_chromosomow,
                           liczba_chromosomow,
                           nr_najlepszego_chr_zadan,
                           nr_najlepszego_chr_zasobow);
    
    chromosom_zadan *chr_zadania_kopia = new chromosom_zadan[liczba_chromosomow];
    chromosom_zasobow *chr_zasoby_kopia = new chromosom_zasobow[liczba_chromosomow];
    
    // Zapisywanie kopii najlepszej pary chromosomów, która wejdzie do nowej populacji
    chr_zadania_kopia[0] = chr_zadania[nr_najlepszego_chr_zadan];
    chr_zasoby_kopia[0] = chr_zasoby[nr_najlepszego_chr_zasobow];                
    
    short *wartosci_f_przystosowania = new short[liczba_chromosomow*liczba_chromosomow];
    unsigned int suma_wartosci_f_przyst;
    
    suma_wartosci_f_przyst = LiczWartosciFPrzystosowania(przyst_chromosomow,
                                                         liczba_chromosomow,
                                                         wartosci_f_przystosowania);
    short prog,
          losowana_wartosc;
    short nr_wylosowanej_pary_chr,
          nr_wylosowanego_chr_zadan,
          nr_wylosowanego_chr_zasobow;
    bool znaleziono_numery_chr;
    
    for(short nr_chr = 1; nr_chr < liczba_chromosomow; nr_chr++)
    {
        prog = 0;
        losowana_wartosc = rand() % suma_wartosci_f_przyst + 1;
        
        for(short nr_przyst = 0;
            nr_przyst < liczba_chromosomow*liczba_chromosomow;
            nr_przyst++)
        {
            prog += wartosci_f_przystosowania[nr_przyst];
            
            if(losowana_wartosc <= prog)
            {
                nr_wylosowanej_pary_chr = nr_przyst;
                break;    
            }
        }

        /* Szukanie numerów chromosomów z obu populajci odpowiadających 
           wylosowanej pozycji na kole ruletki */
        znaleziono_numery_chr = false;
        for(short nr_chr_zadan = 0; nr_chr_zadan < liczba_chromosomow; nr_chr_zadan++)
        {
            for(short nr_chr_zasobow = 0;
                nr_chr_zasobow < liczba_chromosomow;
                nr_chr_zasobow++)
            {
                 if(nr_wylosowanej_pary_chr == nr_chr_zadan*liczba_chromosomow + nr_chr_zasobow)
                 {
                     nr_wylosowanego_chr_zadan = nr_chr_zadan;
                     nr_wylosowanego_chr_zasobow = nr_chr_zasobow;
                     znaleziono_numery_chr = true;
                     break;
                 }
            }      
            if(znaleziono_numery_chr)
                 break;
        }
        
        chr_zadania_kopia[nr_chr] = chr_zadania[nr_wylosowanego_chr_zadan];
        chr_zasoby_kopia[nr_chr] = chr_zasoby[nr_wylosowanego_chr_zasobow];
    }
    
    // Tworzenie nowej populacji
    for(short nr_chr = 0; nr_chr < liczba_chromosomow; nr_chr++)
    {
        chr_zadania[nr_chr] = chr_zadania_kopia[nr_chr];
        chr_zasoby[nr_chr] = chr_zasoby_kopia[nr_chr];
    }
    
    delete[] chr_zadania_kopia;
    delete[] chr_zasoby_kopia;
}
//------------------------------------------------------------------------------
void SelekcjaRankingowa(przystosowanie przyst_chromosomow[],
                        chromosom_zadan chr_zadania[],
                        chromosom_zasobow chr_zasoby[],
                        unsigned short liczba_chromosomow)
{
    short nr_najlepszego_chr_zadan,
          nr_najlepszego_chr_zasobow;
          
    ZnajdzNajlepszaPareChr(przyst_chromosomow,
                           liczba_chromosomow,
                           nr_najlepszego_chr_zadan,
                           nr_najlepszego_chr_zasobow);
    
    chromosom_zadan *chr_zadania_kopia = new chromosom_zadan[liczba_chromosomow];
    chromosom_zasobow *chr_zasoby_kopia = new chromosom_zasobow[liczba_chromosomow];
    
    // Zapisywanie kopii najlepszej pary chromosomów, która wejdzie do nowej populacji
    chr_zadania_kopia[0] = chr_zadania[nr_najlepszego_chr_zadan];
    chr_zasoby_kopia[0] = chr_zasoby[nr_najlepszego_chr_zasobow];                        
                        
    short suma_rang = 0;
    for(short na_pary=0; na_pary<liczba_chromosomow*liczba_chromosomow; na_pary++)
         suma_rang += (na_pary+1);
    
    short liczba_gorszych_wartosci_f_celu;
    bool powtorka_wartosci_f_celu;
    short *rangi = new short[liczba_chromosomow*liczba_chromosomow];
    
    // Ustalanie rang par chromosomów
    for(short nr_rangi = 0; nr_rangi < liczba_chromosomow*liczba_chromosomow; nr_rangi++)
    {
        for(short nr_chr = 0;
            nr_chr < liczba_chromosomow*liczba_chromosomow;
            nr_chr++) // chromosomy bieżące
        { 
            powtorka_wartosci_f_celu = false;
            liczba_gorszych_wartosci_f_celu = 0;
            // Zliczanie wartości funkcji celu gorszych (większych) od bieżącej
            for(short nr_innego_chr = 0;
                nr_innego_chr < liczba_chromosomow*liczba_chromosomow;
                nr_innego_chr++)// chromosomy do porównania
            {
                if(nr_chr != nr_innego_chr 
                   && przyst_chromosomow[nr_innego_chr].PobierzWartoscFCelu()
                   > przyst_chromosomow[nr_chr].PobierzWartoscFCelu())
                    liczba_gorszych_wartosci_f_celu++;
            }
            if(liczba_gorszych_wartosci_f_celu < nr_rangi && nr_rangi)
            {
                 // Sprawdzanie ile porównywanych wart. f. celu ma tę samą wartość co bieżaca
                 powtorka_wartosci_f_celu = false;
                 for(short nr_innego_chr = 0;
                     nr_innego_chr < liczba_chromosomow*liczba_chromosomow;
                     nr_innego_chr++)// chromosomy do porównania
                 {
                     if(nr_chr != nr_innego_chr
                        && przyst_chromosomow[nr_chr].PobierzWartoscFCelu()
                        == przyst_chromosomow[nr_innego_chr].PobierzWartoscFCelu())
                     {
                         powtorka_wartosci_f_celu = true;
                         break;
                     }
                 }
                 /* Jeśli występują powtórki, porównujemy wartości FC z wcześniejszych rang */
                 if(powtorka_wartosci_f_celu)
                 {
                     for(short nr_wczesniejszej_rangi = nr_rangi-1;
                         nr_wczesniejszej_rangi >= 0;
                         nr_wczesniejszej_rangi--)
                     {
                         if(rangi[nr_wczesniejszej_rangi] != nr_chr
                            && przyst_chromosomow[rangi[nr_wczesniejszej_rangi]].PobierzWartoscFCelu()
                            == przyst_chromosomow[nr_chr].PobierzWartoscFCelu())
                             liczba_gorszych_wartosci_f_celu++;      
                     }
                 }
            }
            /* Jeśli większych FC (lub równych w przypadku wielu tych samych wartości FC)
               jest n, znaleziono indeks odpowiadający randze */
            if(liczba_gorszych_wartosci_f_celu == nr_rangi)
            {
                rangi[nr_rangi] = nr_chr;
                break;
            }
        }
    }
    
    short prog,
          losowana_wartosc;
    short nr_wylosowanej_pary;
    bool znaleziono_numery_chr;
    
    for(short nr_chr = 1; nr_chr < liczba_chromosomow; nr_chr++)
    {
        prog = 0;
        /* Losowanie liczby z przedziału od 1 do liczby będącej sumą 
           wszystkich rang par chromosomów (1+2+3+4... = suma_rang) */
        losowana_wartosc = rand() % suma_rang + 1;
        for(short nr_pary = 0;
            nr_pary < liczba_chromosomow*liczba_chromosomow;
            nr_pary++)
        {
            prog += (nr_pary+1);
            if(losowana_wartosc <= prog)
            {
                // indeks wylosowanej pary chromosomów dla FC
                nr_wylosowanej_pary = rangi[nr_pary];
                break;
            }
        }
        // Zapisanie wylosowaneej pary chromosomów
        znaleziono_numery_chr = false;
        for(short nr_chr_zadan = 0;
            nr_chr_zadan < liczba_chromosomow;
            nr_chr_zadan++)
        {
            for(short nr_chr_zasobow = 0;
                nr_chr_zasobow < liczba_chromosomow;
                nr_chr_zasobow++)
            {
                if(nr_wylosowanej_pary
                   == (nr_chr_zadan*liczba_chromosomow + nr_chr_zasobow))
                {
                     chr_zadania_kopia[nr_chr] = chr_zadania[nr_chr_zadan];
                     chr_zasoby_kopia[nr_chr] = chr_zasoby[nr_chr_zasobow];

                     znaleziono_numery_chr = true;
                     break;
                }     
            }
            if(znaleziono_numery_chr)
                break;
        }       
    }
    delete[] rangi;
    
    // Tworzenie nowej populacji
    for(short nr_chr = 0; nr_chr < liczba_chromosomow; nr_chr++)
    {
        chr_zadania[nr_chr] = chr_zadania_kopia[nr_chr];
        chr_zasoby[nr_chr] = chr_zasoby_kopia[nr_chr];
    } 
    
    delete[] chr_zadania_kopia;
    delete[] chr_zasoby_kopia;
}
//------------------------------------------------------------------------------
unsigned int LiczWartosciFPrzystosowania(przystosowanie przyst_chromosomow[],
                                         unsigned short liczba_chromosomow,
                                         short wartosci_f_przystosowania[])
{
    short najw_wartosc_f_celu = 0;
    for(short nr_przyst = 0;
        nr_przyst < liczba_chromosomow*liczba_chromosomow;
        nr_przyst++)
    {
        if(przyst_chromosomow[nr_przyst].PobierzWartoscFCelu() > najw_wartosc_f_celu)
            najw_wartosc_f_celu = przyst_chromosomow[nr_przyst].PobierzWartoscFCelu();
    }
    
    /* Wyznaczenie najmniejszej wielokrotności 10 większej od największej
       spośród wartości funkcji celu.  */
    short najmn_wielokrotnosc_10 = 10;
    while(najw_wartosc_f_celu > najmn_wielokrotnosc_10)
    {
        najmn_wielokrotnosc_10 *= 10;  
    }
    
    /* Zamiana funkcji celu na funkcje przystosowania, by możliwe było dokonanie selekcji
       metodą koła ruletki (dla wartości funkcji celu - im mniejsza wartość funkcji, tym lepiej;
       dla funkcji przystosowania - im większa wartość funkcji, tym lepiej) */
    unsigned int suma_wartosci_f_przyst = 0;
    for(short nr_przyst = 0;
        nr_przyst < liczba_chromosomow*liczba_chromosomow;
        nr_przyst++)
    {   
        wartosci_f_przystosowania[nr_przyst] = najmn_wielokrotnosc_10
                                               - przyst_chromosomow[nr_przyst].PobierzWartoscFCelu();
        suma_wartosci_f_przyst += wartosci_f_przystosowania[nr_przyst];
    }
    
    /* Zamiana wartości funkcji przystosowania na wartości procentowe z dokładnością
       do 0,1%. Gwarantuje to, że sumy kolejnych wartości f. przyst. przy losowaniu
       chromosomu nie przekroczą maksymalnej możliwej do otrzymania liczby przy
       funkcji rand() (RAND_MAX). Możliwe straty przy konwersji float -> short. */
    for(short nr_przyst = 0; nr_przyst < liczba_chromosomow*liczba_chromosomow; nr_przyst++)
    {   
        wartosci_f_przystosowania[nr_przyst] = static_cast<short>
                                               ((static_cast<float>(wartosci_f_przystosowania[nr_przyst])
                                                 / static_cast<float>(suma_wartosci_f_przyst)) * 1000.0);
    }
    /* Przemianowanie sumy wartości f. przystosowania na potrzeby losowania pary chromosomów.
       Suma wyniesie zawsze wartość mniejszą lub równą 1000 */
    suma_wartosci_f_przyst = 0;
    for(short nr_przyst = 0; nr_przyst < liczba_chromosomow*liczba_chromosomow; nr_przyst++)
    {   
        suma_wartosci_f_przyst += wartosci_f_przystosowania[nr_przyst];
    }
    
    return suma_wartosci_f_przyst;
}
