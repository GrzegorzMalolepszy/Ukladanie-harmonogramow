#include "przystosowanie.h"
#include <iostream>
#include <list>
using namespace std;

void przystosowanie::LiczFunkcjeCelu(chromosom_zadan &chr_zadania,
                                     chromosom_zasobow &chr_zasoby)
{
    unsigned short min_czas_rozpoczecia;
    unsigned short liczba_zajetych_zasobow,
                   liczba_wolnych_zasobow,
                   liczba_brakujacych_zasobow;
    unsigned short nr_potrzebnego_zasobu;
    unsigned short nr_zespolu;
    unsigned short najwiecej_w_zespole = zespol::PobierzNajwiecejWZespole();
    
    unsigned short *czasy_ukonczenia = new unsigned short[liczba_zadan];
    unsigned short **zajetosc_zasobow = new unsigned short *[liczba_zespolow];
    for(nr_zespolu = 0; nr_zespolu < liczba_zespolow; nr_zespolu++)
    {
        zajetosc_zasobow[nr_zespolu] = new unsigned short[najwiecej_w_zespole];
        for(short nr_zasobu = 0; nr_zasobu < najwiecej_w_zespole; nr_zasobu++)
             zajetosc_zasobow[nr_zespolu][nr_zasobu] = 0;
    }
    
    wartosc_f_celu = 0;
    
    for(short nr_genu = 0; nr_genu < liczba_zadan; nr_genu++)
    {
        min_czas_rozpoczecia = 0; 
        for(short nr_poprz_genu = nr_genu-1; nr_poprz_genu >= 0; nr_poprz_genu--)//zadania poprzednie w kolejce
        {     
            if(lista_zadan[chr_zadania.geny[nr_genu]].CzyToPoprzednik(chr_zadania.geny[nr_poprz_genu]))
            {
                /* minimalny czas rozpoczęcia bieżącego zadania
                 = czas zakończenia zadania, będącego bezpośrednim poprzednikiem */  
                min_czas_rozpoczecia = czasy_ukonczenia[nr_poprz_genu];                                                                 
                break;
            }
                
        }   
        
        // Zliczanie zajętych zasobów
        liczba_zajetych_zasobow = 0;
        nr_zespolu = lista_zadan[chr_zadania.geny[nr_genu]].PobierzNrZespolu();
        for(short nr_zasobu = 0;
            nr_zasobu < najwiecej_w_zespole;
            nr_zasobu++)
        {
            if(zajetosc_zasobow[nr_zespolu][nr_zasobu])
                liczba_zajetych_zasobow++;
        }

        liczba_wolnych_zasobow = lista_zespolow[nr_zespolu].PobierzLiczbePracownikow()
            - liczba_zajetych_zasobow;
        
        // Zliczanie zajętych zasobów, które potrzebne są do wykonania bieżącego zadania
        if(chr_zasoby.geny[chr_zadania.geny[nr_genu]] > liczba_wolnych_zasobow )
        {
            liczba_brakujacych_zasobow = chr_zasoby.geny[chr_zadania.geny[nr_genu]]
                                       - liczba_wolnych_zasobow;
        }
        else
            liczba_brakujacych_zasobow = 0;
                           
        // Jeśli do wykonania zadania potrzeba zajętych pracowników
        if(liczba_brakujacych_zasobow)
        {   
            while(liczba_brakujacych_zasobow)
            {
                // Znalezienie pierwszego zajętego zasobu
                for(short nr_zasobu = 0; nr_zasobu < najwiecej_w_zespole; nr_zasobu++)
                {
                    if(zajetosc_zasobow[nr_zespolu][nr_zasobu])
                    {
                        nr_potrzebnego_zasobu = nr_zasobu;
                        break;
                    }
                }    
                
                // Znajdowanie potrzebnych zasobów, które jako pierwsze się zwolnią
                for(short nr_zasobu = 0; nr_zasobu < najwiecej_w_zespole; nr_zasobu++)
                {
            
                     if(zajetosc_zasobow[nr_zespolu][nr_zasobu]
                        && zajetosc_zasobow[nr_zespolu][nr_zasobu]
                           < zajetosc_zasobow[nr_zespolu][nr_potrzebnego_zasobu])
                     {
                         nr_potrzebnego_zasobu = nr_zasobu;
                     }
                }
                
                if(min_czas_rozpoczecia < zajetosc_zasobow[nr_zespolu][nr_potrzebnego_zasobu])
                     min_czas_rozpoczecia = zajetosc_zasobow[nr_zespolu][nr_potrzebnego_zasobu]; 
                         
                zajetosc_zasobow[nr_zespolu][nr_potrzebnego_zasobu] = 0;
                
                liczba_brakujacych_zasobow--;
            }
        }
        
        czasy_ukonczenia[nr_genu] = min_czas_rozpoczecia
                                    + (lista_zadan[chr_zadania.geny[nr_genu]].PobierzRoboczodni()
                                       / chr_zasoby.geny[chr_zadania.geny[nr_genu]]);
        
        // Dodawanie nowej informacji o zajętości czasowej zasobów
        for(short nr_potrzebnego_zasobu = 0;
            nr_potrzebnego_zasobu < chr_zasoby.geny[chr_zadania.geny[nr_genu]];
            nr_potrzebnego_zasobu++)
        {
            for(short nr_zasobu = 0;
                nr_zasobu < najwiecej_w_zespole;
                nr_zasobu++ )
            {
                if(zajetosc_zasobow[nr_zespolu][nr_zasobu] == 0)
                {
                     zajetosc_zasobow[lista_zadan[chr_zadania.geny[nr_genu]].PobierzNrZespolu()][nr_zasobu]
                     = czasy_ukonczenia[nr_genu]; 
                     break;
                }
            }
        }
    }
    
    for(short nr_genu=0; nr_genu < liczba_zadan; nr_genu++)
    {
        if(wartosc_f_celu < czasy_ukonczenia[nr_genu])
             wartosc_f_celu = czasy_ukonczenia[nr_genu];
    }
    
    delete [] czasy_ukonczenia;
    for(short nr_zespolu=0; nr_zespolu < liczba_zespolow; nr_zespolu++)
        delete [] zajetosc_zasobow[nr_zespolu];
    delete [] zajetosc_zasobow;
}
