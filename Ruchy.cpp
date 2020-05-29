#include"Ruchy.h"
#include<Windows.h>

using namespace std;

bool przesun(koordy srodki[10][5], koordy pozycje[10][5], Tobiekty &piony, sf::RenderWindow &okno, sf::Sprite stlo, po_ruchu &paczka, bool gracz)
{
	int x, y, nr=-1, nrs=-1, pi=-1, pj=-1;
	sf::Event zdarzenie;

	while(1)
	{
		x=sf::Mouse::getPosition(okno).x; y=sf::Mouse::getPosition(okno).y;

	/*Identyfikujemy, który pion zosta³ wybrany myszk¹.*/
		if(nr==-1) for(nr=0; nr<50; nr++) if(srodki[nr/5][nr%5].typ!=-1)
			if(x<srodki[nr/5][nr%5].x+r_piona&&x>srodki[nr/5][nr%5].x-r_piona)
				if(y<srodki[nr/5][nr%5].y+r_piona&&y>srodki[nr/5][nr%5].y-r_piona)
				{ nrs=nr; nr=srodki[nr/5][nr%5].typ; break; }

	/*Gdy puszczamy lewy myszki, to akywuje siê funkcja wyrównuj¹ca pozycjê piona
	i przypisujaca go do nowej pozycji w tablicy œrodki.*/
		okno.pollEvent(zdarzenie);
		if(zdarzenie.type==sf::Event::MouseButtonReleased&&zdarzenie.mouseButton.button==sf::Mouse::Left) 
		{
			if(nr<(int)piony.size()) for(int i=0; i<10; i++) 
			{
				for(int j=0; j<5; j++)
					if((piony[nr].x<pozycje[i][j].x+r_piona&&piony[nr].x>pozycje[i][j].x-r_piona)
						&&(piony[nr].y<pozycje[i][j].y+r_piona&&piony[nr].y>pozycje[i][j].y-r_piona))
					{
						if(!piony[nr].typ){if(!dozwolony_ruch(i, j, nrs, piony, srodki)){ i=9; break; }}
						else if(!dozwolona_dama(i, j, nrs, piony, srodki)){i=9; break;}
						else if(dozwolona_dama(i, j, nrs, piony, srodki)==2){paczka.bylo_bicie=true; }

						piony[nr].x=pozycje[i][j].x; piony[nr].y=pozycje[i][j].y;
						/*srodki[nrs/5][nrs%5].typ=-1;*/ srodki[i][j].typ=nr;
						srodki[i][j].gracz=piony[nr].gracz; paczka.focus=i*5+j;

						
						if(paczka.bylo_bicie) bicie(i, (j+nrs%5)/2, nrs, srodki, piony);/*To jest bicie dam¹*/
						else if(czy_bicie(i, j, nrs, srodki, piony)){ bicie(i, (j+nrs%5)/2, nrs, srodki, piony); paczka.bylo_bicie=true; }
						else paczka.bylo_bicie=false;
						srodki[nrs/5][nrs%5].typ=-1;

						i=10; return true;
					}
				if(i==9){ piony[nr].x=pozycje[nrs/5][nrs%5].x; piony[nr].y=pozycje[nrs/5][nrs%5].y; }
			}
			break;
		}

		if(nr>(int)(piony.size()-1)){ nr=-1; continue; }
		if(piony[nr].gracz!=gracz) break;
		piony[nr].x=x-r_piona; piony[nr].y=y-r_piona;
		
		okno.clear(); okno.draw(stlo);
		rysuj_obiekty(piony, okno);
		okno.display();
	}
	return false;
}

bool dozwolony_ruch(int i, int j, int nrs, Tobiekty piony, koordy srodki[10][5])
{
	if(srodki[i][j].typ!=-1) return false;
	if(abs(srodki[i][j].x-srodki[nrs/5][nrs%5].x)!=abs(srodki[i][j].y-srodki[nrs/5][nrs%5].y)) return false;

	if(!czy_bicie(i, j, nrs, srodki, piony))
	{
		if(i>nrs/5+1||i<=nrs/5) return false;

		return !jakiekolwiek_bicie(nrs, srodki, piony);//if(jakiekolwiek_bicie(nrs, srodki)) return false; jbc
	}

	return true;
}

/*true - ruch dozwolony; false - niedozwolony; 2 - by³o bicie*/
int dozwolona_dama(int i, int j, int nrs, Tobiekty piony, koordy srodki[10][5])
{
	if(srodki[i][j].typ!=-1) return false;
	if(abs(srodki[i][j].x-srodki[nrs/5][nrs%5].x)!=abs(srodki[i][j].y-srodki[nrs/5][nrs%5].y)) return false;

	bool jeden_pion=false;
	int ykier=(i-nrs/5)/abs(i-nrs/5), xkier=0;
	if(j!=nrs%5)xkier=(j-nrs%5)/abs(j-nrs%5);

	int y=nrs/5+ykier; int x=nrs%5-y%2;
	while(y!=i)
	{
		if(srodki[y][x].typ!=-1)
			if(srodki[y][x].gracz==srodki[nrs/5][nrs%5].gracz) return false;
			else
			{
				if(jeden_pion) return false;
				else if(y+ykier==i) return 2; 
				else jeden_pion=true;
			}

		y+=ykier; if(!(y%2)) x+=xkier;
	}
	return true;
}

bool czy_bicie(int i, int j, int nrs, koordy srodki[10][5], Tobiekty piony)
{
	if(i!=nrs/5+2&&i!=nrs/5-2) return false;
	if(i<nrs/5) i+=2;

	if((nrs/5)%2) j++;
	if(srodki[i-1][(j+nrs%5)/2].typ==-1) return false;
	if(srodki[i-1][(j+nrs%5)/2].gracz==srodki[nrs/5][nrs%5].gracz) return false;

	return true;
}

bool jakiekolwiek_bicie(int nrs, koordy srodki[10][5], Tobiekty piony)
{
	int tmp;
	for(int s=0; s<50; s++)
	{
		tmp=srodki[s/5][s%5].typ;
		if(tmp==-1||srodki[s/5][s%5].gracz!=srodki[nrs/5][nrs%5].gracz) continue;
		if(s/5+2<10)
		{
			if(s%5+1<5) if(srodki[s/5+2][s%5+1].typ==-1) if(czy_bicie(s/5+2, s%5+1, s, srodki, piony)) return true;
			if(s%5-1>-1) if(srodki[s/5+2][s%5-1].typ==-1) if(czy_bicie(s/5+2, s%5-1, s, srodki, piony)) return true;
		}

		if(s/5-2>-1)
		{
			if(s%5+1<5) if(srodki[s/5-2][s%5+1].typ==-1) if(czy_bicie(s/5-2, s%5+1, s, srodki, piony)) return true;
			if(s%5-1>-1) if(srodki[s/5-2][s%5-1].typ==-1) if(czy_bicie(s/5-2, s%5-1, s, srodki, piony)) return true;
		}
	}

	return false;
}

void bicie(int i, int j, int nrs, koordy srodki[10][5], Tobiekty &piony)
{
	if(!(abs(i-nrs/5)%2)) {if(i%2&&(i%2==(nrs/5)%2))j++;}

	if(i<nrs/5)i+=2;

	piony.erase(piony.begin()+srodki[i-1][j].typ);
	for(int x=0; x<10; x++)
		for(int y=0; y<5; y++)
			if(srodki[x][y].typ>srodki[i-1][j].typ) srodki[x][y].typ--;

	srodki[i-1][j].typ=-1;
}