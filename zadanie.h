#ifndef ZADANIE_H
#define ZADANIE_H
#include <string>
#include <list>

extern unsigned short liczba_zespolow;

class zadanie
{
private:
    static short liczba_obiektow;
    unsigned short id;
    std::string nazwa;
    unsigned short roboczodni;
    unsigned short nr_zespolu;
    std::list<zadanie *> poprzedniki;
public:
    zadanie();
    ~zadanie();
    void PodajCechyZadania(std::string nazwa_zadania,
                           unsigned short roboczodni_zadania,
                           unsigned short nr_przydz_zespolu)
    {
         nazwa = nazwa_zadania;
         roboczodni = roboczodni_zadania;
         nr_zespolu = nr_przydz_zespolu;
    }
    void DodajPoprzednika(zadanie *adres_zadania)
    {
         poprzedniki.push_back(adres_zadania);
    }
    unsigned short PobierzId()
    {
         return id;  
    }
    std::string PobierzNazwe()
    {
         return nazwa;  
    }
    unsigned short PobierzRoboczodni()
    { 
         return roboczodni;
    }
    unsigned short PobierzNrZespolu()
    {
         return nr_zespolu;
    }
    void LosujZadanie(zadanie lista_zadan[],
                      unsigned short maks_roboczodni,
                      unsigned short min_roboczodni);
    bool CzyMaPoprzednikow()
    {
         return !poprzedniki.empty();
    }
    bool CzyToPoprzednik(short nr_zadania_poprzedzajacego);
    std::string ListaPoprzednikowJakoString();
    
};
#endif
