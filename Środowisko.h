#pragma once
#ifndef �rodowisko

#include<SFML/Graphics.hpp>
#include<vector>

using namespace std;

/*Parametry wielko�ci poszczeg�lnych element�w*/
const int r_pola=61;
const int r_okna=616;
const int r_piona=22;
const sf::Vector2f r_przycisku(150, 50);

/*Og�lna struktura przechowuj�ca parametry piona tudzie� pola*/
struct koordy
{
/*Dla pion�w typ: 0 = pion, 1 = dama; Dla pozycji: -1 = wolna, 'cyfra' = numer piona, kt�ry si� na niej znajduje*/
	int x, y; int typ; bool gracz;
	koordy(int X=0, int Y=0, int D=0, bool G=0) : x(X), y(Y), typ(D), gracz(G) {}
};

/*Struktura przechowuj�ca parametry wykonanego ruchu*/
struct po_ruchu
{
	int focus, focus_nr; bool bylo_bicie;
	po_ruchu(int F=-1, int F2=-1, bool B=0): focus(F), focus_nr(F2), bylo_bicie(B){}
};

/*Typ dla listy(wektora) przechowuj�cego wszystkie piony, znajduj�ce si� na planszy*/
typedef vector<koordy> Tobiekty;

/*Funkcje �rodowiskowe - dotycz�ce obs�ugi planszy oraz efekt�w wizualnych*/
int pseudomenu(sf::Font, sf::Sprite, sf::RenderWindow& okno);
void rysuj_obiekty(Tobiekty, sf::RenderWindow& okno);
void poz_wyjsciowe(koordy pozycje[10][5], koordy srodki[10][5], Tobiekty &);
void poz_testowe(koordy pozycje[10][5], koordy srodki[10][5], Tobiekty &);
void zmien_gracza(koordy srodki[10][5], koordy pozycje[10][5], Tobiekty &, sf::RenderWindow &); //Czyli obr�� plansz�.


#endif