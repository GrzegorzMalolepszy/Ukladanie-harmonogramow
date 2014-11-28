#ifndef CHROMOSOM_ZASOBOW_H
#define CHROMOSOM_ZASOBOW_H
#include "zadanie.h"
#include "zespol.h"

extern unsigned short liczba_zadan,
                      liczba_zespolow;
extern zadanie *lista_zadan;
extern zespol *lista_zespolow;

class chromosom_zasobow
{ 
private:
     short *geny;
     
     friend class przystosowanie;
     friend class harmonogram_txt;
public:
     chromosom_zasobow();
     chromosom_zasobow(chromosom_zasobow &wzor_chr);
     ~chromosom_zasobow();
     chromosom_zasobow & operator=(const chromosom_zasobow &wzor_chr);
     void KopiujGeny(short kopia_genow[]);
     void LosujChrZasobow();
     
     void KrzyzowanieJednopunktowe(chromosom_zasobow &rodzic1,
                                   chromosom_zasobow &rodzic2);
     void KrzyzowanieDwupunktowe(chromosom_zasobow &rodzic1,
                                 chromosom_zasobow &rodzic2);
     void KrzyzowanieJednorodne(chromosom_zasobow &rodzic1,
                                chromosom_zasobow &rodzic2);
     void MutacjaZmianaWartosci(const unsigned short prawdop_mutacji);
};
#endif
