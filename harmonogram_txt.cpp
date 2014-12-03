#include "harmonogram_txt.h"
#include <iostream>
#include <list>
#include <string>
#include <windows.h>
#include <sstream>
#include <fstream>
using namespace std;

bool harmonogram_txt::TworzHarmonogram(chromosom_zadan &chr_zadania,
                                       chromosom_zasobow &chr_zasobu)
{   
    unsigned short wszyscy_pracownicy = zespol::PobierzLiczbePracownikowOgolem();
    
    bool **wykres_zadan   = new bool *[czas_realizacji];
    bool **wykres_zasobow = new bool *[czas_realizacji];   
    for(short dzien = 0; dzien < czas_realizacji; dzien++)
    {
        wykres_zadan[dzien] = new bool[liczba_zadan];
        wykres_zasobow[dzien] = new bool[wszyscy_pracownicy];
        for(short nr_zadania = 0; nr_zadania < liczba_zadan; nr_zadania++)
        {
             wykres_zadan[dzien][nr_zadania] = 0;
        }
        for(short nr_pracownika = 0;
            nr_pracownika < wszyscy_pracownicy;
            nr_pracownika++)
        {
             wykres_zasobow[dzien][nr_pracownika] = 0;
        }
    }
    
    unsigned short min_czas_rozpoczecia;
    unsigned short liczba_zajetych_zasobow,
                   liczba_wolnych_zasobow,
                   liczba_brakujacych_zasobow;
    unsigned short nr_potrzebnego_zasobu;
    unsigned short nr_zespolu;
    unsigned short poczatek_zazn_zasobow;
    unsigned short najwiecej_w_zespole = zespol::PobierzNajwiecejWZespole();

    unsigned short *czasy_ukonczenia = new unsigned short[liczba_zadan];
    unsigned short **zajetosc_zasobow = new unsigned short *[liczba_zespolow];
    for(nr_zespolu = 0; nr_zespolu < liczba_zespolow; nr_zespolu++)
    {
        zajetosc_zasobow[nr_zespolu] = new unsigned short[najwiecej_w_zespole];
        for(short nr_zasobu = 0; nr_zasobu < najwiecej_w_zespole; nr_zasobu++)
             zajetosc_zasobow[nr_zespolu][nr_zasobu] = 0;
    }
    
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
        if(chr_zasobu.geny[chr_zadania.geny[nr_genu]] > liczba_wolnych_zasobow )
        {
            liczba_brakujacych_zasobow = chr_zasobu.geny[chr_zadania.geny[nr_genu]]
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
                                       / chr_zasobu.geny[chr_zadania.geny[nr_genu]]);
        
        for(short jedn_czasu = czasy_ukonczenia[nr_genu];
            jedn_czasu > czasy_ukonczenia[nr_genu]
                         - (lista_zadan[chr_zadania.geny[nr_genu]].PobierzRoboczodni()
                            / chr_zasobu.geny[chr_zadania.geny[nr_genu]]);
            jedn_czasu--)
        {
            wykres_zadan[jedn_czasu-1][nr_genu] = 1;
        }
        
        // Dodawanie nowej informacji o zajętości czasowej zasobów
        for(short nr_potrzebnego_zasobu = 0;
            nr_potrzebnego_zasobu < chr_zasobu.geny[chr_zadania.geny[nr_genu]];
            nr_potrzebnego_zasobu++)
        {
            /* Znajdowanie miejsca na harmonogramie, od którego należy rozpocząć
               zaznaczanie zajętości zasobu */
            poczatek_zazn_zasobow = 0;
            for(short nr_zazn_zespolu = 0;
                nr_zazn_zespolu <= nr_zespolu-1;
                nr_zazn_zespolu++)
            {
                poczatek_zazn_zasobow += lista_zespolow[nr_zazn_zespolu].PobierzLiczbePracownikow();    
            }
            
            for(short nr_zasobu = 0;
                nr_zasobu < najwiecej_w_zespole;
                nr_zasobu++ )
            {
                if(zajetosc_zasobow[nr_zespolu][nr_zasobu] == 0)
                {
                     zajetosc_zasobow[nr_zespolu][nr_zasobu]
                     = czasy_ukonczenia[nr_genu]; 
                     
                     // Zaznaczanie zajętości zasobu na wykresie
                     for(short jedn_czasu = czasy_ukonczenia[nr_genu];
                         jedn_czasu > czasy_ukonczenia[nr_genu]
                                      -(lista_zadan[chr_zadania.geny[nr_genu]].PobierzRoboczodni()
                                        / chr_zasobu.geny[chr_zadania.geny[nr_genu]]);
                         jedn_czasu--)
                     {
                         wykres_zasobow[jedn_czasu-1][poczatek_zazn_zasobow + nr_zasobu] = 1;
                     }
                     
                     break;
                }
            }
        }
    }
    
    delete [] czasy_ukonczenia;
    for(short nr_zespolu = 0; nr_zespolu < liczba_zespolow; nr_zespolu++)
        delete [] zajetosc_zasobow[nr_zespolu];
    delete [] zajetosc_zasobow;
    
    bool pomyslnie_wyeksportowano = ZapiszDoPliku(chr_zadania,
                                                  chr_zasobu,
                                                  wykres_zadan,
                                                  wykres_zasobow);
    for(short dzien = 0; dzien < czas_realizacji; dzien++)
    {
        delete [] wykres_zadan[dzien];
        delete [] wykres_zasobow[dzien];
    }
    delete [] wykres_zadan;
    delete [] wykres_zasobow;
    
    return pomyslnie_wyeksportowano;
}
//------------------------------------------------------------------------------
bool harmonogram_txt::ZapiszDoPliku(chromosom_zadan &chr_zadania,
                                    chromosom_zasobow &chr_zasobu,
                                    bool ** &wykres_zadan,
                                    bool ** &wykres_zasobow)
{
    string nazwa_pliku = nazwa + ".txt";
    DeleteFile(nazwa_pliku.c_str());
    /* Tworzymy plik, jeśli nie istnieje (flaga CREATE_NEW)
    Funkcja zwraca uchwyt do pliku, więc od razu go zamykamy, bo go nie
    potrzebujemy */
    CloseHandle(CreateFile(nazwa_pliku.c_str(), 
                GENERIC_READ | GENERIC_WRITE, 
                FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
                NULL,
                CREATE_NEW,
                FILE_ATTRIBUTE_NORMAL,
                NULL)); 
    
    fstream plik;
    plik.open(nazwa_pliku.c_str(), fstream::in | fstream::out);
    
    if(!plik.is_open())
        return false;
    
    // Dane wstępne
    plik << "Zadanie\tNazwa\tRoboczodni\tNr zespołu\tPrzydzieleni pracownicy\tPoprzedniki\n";
    for(short nr_zadania = 0; nr_zadania < liczba_zadan; nr_zadania++)
    {
        plik << nr_zadania+1 << "\t";
        plik << lista_zadan[chr_zadania.geny[nr_zadania]].PobierzNazwe() << "\t";
        plik << lista_zadan[chr_zadania.geny[nr_zadania]].PobierzRoboczodni() << "\t";
        plik << lista_zadan[chr_zadania.geny[nr_zadania]].PobierzNrZespolu()+1 << "\t";
        plik << chr_zasobu.geny[chr_zadania.geny[nr_zadania]] << "\t";
        plik << lista_zadan[chr_zadania.geny[nr_zadania]].ListaPoprzednikowJakoString();
        plik << "\n";
    }
    plik << "\n";
    
    // Pierwszy wiersz harmonogramu
    plik << "\tZadania";
    for(short nr_zadania = 0; nr_zadania < liczba_zadan; nr_zadania++)
    {
        plik << '\t';
    }
    for(short nr_zespolu = 0; nr_zespolu < liczba_zespolow; nr_zespolu++)
    {
        plik << "Zespół " << nr_zespolu+1;
        for(short nr_pracownika = 0;
            nr_pracownika < lista_zespolow[nr_zespolu].PobierzLiczbePracownikow();
            nr_pracownika++)
        {
            plik << '\t';
        }
    }
    plik << "\n\t";
    
    // Drugi wiersz harmonogramu
    for(short nr_zadania = 1; nr_zadania <= liczba_zadan; nr_zadania++)
    {
        plik << nr_zadania << '\t';
    }
    for(short nr_zespolu = 0; nr_zespolu < liczba_zespolow; nr_zespolu++)
    {
        for(short nr_pracownika = 1;
            nr_pracownika <= lista_zespolow[nr_zespolu].PobierzLiczbePracownikow();
            nr_pracownika++)
        {
            plik << nr_pracownika << '\t';
        }
    }
    plik << '\n';
    
    // Zajętość czasowa
    unsigned short wszyscy_pracownicy = zespol::PobierzLiczbePracownikowOgolem();
    for(short jedn_czasu = 0; jedn_czasu < czas_realizacji; jedn_czasu++)
    {
        plik << jedn_czasu+1 << '\t';
        for(short nr_zadania = 0; nr_zadania < liczba_zadan; nr_zadania++)
        {
             if(wykres_zadan[jedn_czasu][nr_zadania])
                 plik << 'o';
             plik << '\t';
        }     
        for(short nr_pracownika = 0;
            nr_pracownika < wszyscy_pracownicy;
            nr_pracownika++)
        {
             if(wykres_zasobow[jedn_czasu][nr_pracownika])
                 plik << 'o';
             plik << '\t';
        }  
        plik << '\n';  
    }
    plik.close();
    
    return true;
}
