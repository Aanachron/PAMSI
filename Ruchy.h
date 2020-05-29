#pragma once
#ifndef Ruchy
#include"Œrodowisko.h"

using namespace std;

bool przesun(koordy srodki[10][5], koordy pozycje[10][5], Tobiekty &, sf::RenderWindow &, sf::Sprite, po_ruchu&, bool gracz=0);
bool dozwolony_ruch(int, int, int, Tobiekty, koordy srodki[10][5]);
int dozwolona_dama(int, int, int, Tobiekty, koordy srodki[10][5]);
bool czy_bicie(int, int, int, koordy srodki[10][5], Tobiekty);
bool jakiekolwiek_bicie(int, koordy srodki[10][5], Tobiekty);
void bicie(int, int, int, koordy srodki[10][5], Tobiekty &);

#endif