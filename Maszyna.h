#pragma once
#ifndef Maszyna
#include"Œrodowisko.h"
#include"Ruchy.h"

void ruch_komputera(int i, int j, int nrs, koordy pozycje[10][5], koordy srodki[10][5], Tobiekty &, sf::RenderWindow &, sf::Sprite);
void normalizuj(int &, int &, int &, int &, koordy srodki[10][5]);
koordy param_bicia(int, int, int, koordy srodki[10][5], Tobiekty);
void bicie_maszyny(int, int, koordy srodki[10][5], Tobiekty &);
#endif