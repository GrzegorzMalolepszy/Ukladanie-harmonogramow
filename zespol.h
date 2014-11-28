#ifndef ZESPOL_H
#define ZESPOL_H
#include <string>

class zespol
{
private:
    static short liczba_obiektow;
    unsigned short id;
    std::string nazwa;
    unsigned short liczba_pracownikow;
public:
    static unsigned short najwiecej_w_zespole;

    zespol();
    ~zespol();
    void PodajCechyZespolu(std::string nazwa_zespolu, unsigned short ile_pracownikow)
    {
         nazwa = std::string(nazwa_zespolu);
         liczba_pracownikow = ile_pracownikow;
         if(liczba_pracownikow > najwiecej_w_zespole)
              najwiecej_w_zespole = liczba_pracownikow;
    }
    unsigned short PobierzId()
    {
         return id;  
    }
    std::string PobierzNazwe()
    {
         return nazwa;  
    }
    unsigned short PobierzLiczbePracownikow()
    {
         return liczba_pracownikow;  
    }
    void LosujZespol(unsigned short maks_zespol, unsigned short min_zespol = 1);
};
#endif
