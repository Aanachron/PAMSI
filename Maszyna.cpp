#include"Maszyna.h"
#include<thread>
#include<Windows.h>
#include<time.h>
#include<cstdlib>

using namespace std;

void ruch_komputera(int i, int j, int nrs, koordy pozycje[10][5], koordy srodki[10][5], Tobiekty &piony, sf::RenderWindow &okno, sf::Sprite stlo)
{
	if(i==nrs/5) return;

	int nr=srodki[nrs/5][nrs%5].typ;
	int x_kier, y_kier;
	x_kier=(pozycje[i][j].x-pozycje[nrs/5][nrs%5].x)/abs(pozycje[i][j].x-pozycje[nrs/5][nrs%5].x);
	y_kier=(pozycje[i][j].y-pozycje[nrs/5][nrs%5].y)/abs(pozycje[i][j].y-pozycje[nrs/5][nrs%5].y);

	while(piony[nr].x!=pozycje[i][j].x&&piony[nr].y!=pozycje[i][j].y)
	{
		piony[nr].x+=x_kier; piony[nr].y+=y_kier;

		okno.clear(); okno.draw(stlo);
		rysuj_obiekty(piony, okno);
		okno.display(); Sleep(5);
	};

	okno.clear(); okno.draw(stlo);
	rysuj_obiekty(piony, okno);
	okno.display(); Sleep(5);

	srodki[nrs/5][nrs%5].typ=-1; srodki[i][j].typ=nr; srodki[i][j].gracz=1;
}

void normalizuj(int &nr, int &focus, int &x, int &y, koordy srodki[10][5])
{
	for(int i=0; i<10; i++)
		for(int j=0; j<5; j++) if(srodki[i][j].typ==nr)focus=i*5+j;
				x=9-x; y=4-y;
}

koordy param_bicia(int i, int j, int nrs, koordy srodki[10][5], Tobiekty piony)
{
	koordy param;

	if(abs(i-nrs/5)>2){ if(!(i%2))j++; }
	else if(i%2) j++;
	if(i<nrs/5)i+=2;

	if(abs(i-nrs/5)<3&&j>nrs%5)j--;
	param.x=9-(i-1); param.y=4-j;
	return param;
}

void bicie_maszyny(int i, int j, koordy srodki[10][5], Tobiekty &piony)
{
	piony.erase(piony.begin()+srodki[i][j].typ);
	for(int x=0; x<10; x++)
		for(int y=0; y<5; y++)
			if(srodki[x][y].typ>srodki[i][j].typ) srodki[x][y].typ--;

	srodki[i][j].typ=-1;
}
