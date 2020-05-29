#include"Œrodowisko.h"
#include"Ruchy.h"
#include"Maszyna.h"
#include<Windows.h>
#include<thread>

using namespace std;

/*int nr=0;
char w[10];
sprintf_s(w, "%d", nr);
MessageBox(NULL, w, "", 0);*/

koordy pozycje[10][5];
koordy srodki[10][5];
Tobiekty piony;

sf::RenderWindow okno(sf::VideoMode(r_okna, r_okna, 32), "Warcaby polonijne", sf::Style::Close);
sf::Texture tlo; sf::Sprite stlo; 
sf::Font czcionka; sf::Text tekst;

void tablice();
void wyrysuj();
bool koniec();

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
	setlocale(LC_ALL, "polish"); tablice(); int gracz=0, tryb=0; sf::Event zdarzenie; po_ruchu paczka; 
	koordy komputera; koordy param_komputera; int losi, losj;
	tlo.loadFromFile("Warcabownica.png");
	stlo.setTexture(tlo); stlo.setPosition(0, 0);
	
	czcionka.loadFromFile("arial.ttf"); tekst.setFillColor(sf::Color::Red);
	tekst.setFont(czcionka); tekst.setPosition(r_okna/2-200, r_okna/2-50); 
	tekst.setCharacterSize(40); tekst.setStyle(sf::Text::Bold);

/*Pseudomenu pozwalaj¹ce na wybór jednego z trzech trybów gry*/
	tryb=pseudomenu(czcionka, stlo, okno);
	if(tryb) poz_wyjsciowe(pozycje, srodki, piony);
	else poz_testowe(pozycje, srodki, piony);

/*G³ówna pêtla aktywna, dopóki okno jest otwarte*/
	while(okno.isOpen())
	{
	/*Tu obs³ugiwane s¹ wszystkie zdarzenia*/
		while(okno.pollEvent(zdarzenie))
		{
			if(zdarzenie.type==sf::Event::Closed) okno.close();
			if(zdarzenie.type==sf::Event::KeyPressed&&zdarzenie.key.code==sf::Keyboard::Escape) okno.close();
			if(zdarzenie.type==sf::Event::MouseButtonPressed&&zdarzenie.mouseButton.button==sf::Mouse::Left)
				if(przesun(srodki, pozycje, piony, okno, stlo, paczka, gracz))
				{
				/*Ruch gracza*/
					wyrysuj(); Sleep(500);

					paczka.focus_nr=srodki[paczka.focus/5][paczka.focus%5].typ;
					if(piony[paczka.focus_nr].y==pozycje[9][0].y)
						piony[paczka.focus_nr].typ=1;

					if(koniec()){ Sleep(500); return 0;}

				/*Jeœli nast¹pi³o bicie i jset */
					if(!paczka.bylo_bicie||paczka.bylo_bicie&&!jakiekolwiek_bicie(paczka.focus, srodki, piony))
					{
						zmien_gracza(srodki, pozycje, piony, okno); gracz=!gracz; 
						if(tryb==1)
						{
							if(gracz) tekst.setString("Ruch gracza czarnego");
							else tekst.setString("Ruch gracza bialego");

							okno.draw(tekst); okno.display();  Sleep(800);
						}
					}
					else if(tryb==2){ paczka.bylo_bicie=false; continue; }

					paczka.bylo_bicie=false;

				/*Ruch komputera*/
					if(tryb==2)
					{
						while(1)
						{
						/*Wartoœci pocz¹tkowe dla losowania*/
							srand((unsigned int)time(NULL));
							paczka.focus_nr=rand()%piony.size();
							losi=rand()%10; losj=rand()%5;
							komputera.typ=0;

							while(1)
							{
								if(paczka.focus_nr==piony.size())paczka.focus_nr=0;
								if(piony[paczka.focus_nr].gracz!=gracz)
								{
									paczka.focus_nr++;
									continue;
								}

								for(int i=0; i<10; i++)
									for(int j=0; j<5; j++)
										if(srodki[i][j].typ==paczka.focus_nr)paczka.focus=i*5+j;

							/*Po wybraniu odpowiedniego piona, sprawdzane s¹ mo¿liwe posuniêcia*/
								for(komputera.x=losi+1; ; komputera.x++)
								{
									if(komputera.x==10) komputera.x=0;
									for(komputera.y=losj+1; ; komputera.y++)
									{
										if(komputera.y==5) komputera.y=0;
										if(piony[paczka.focus_nr].typ) 
											{if(dozwolona_dama(komputera.x, komputera.y, paczka.focus, piony, srodki))
												komputera.typ=1; break;}
										else if(dozwolony_ruch(komputera.x, komputera.y, paczka.focus, piony, srodki))
											{komputera.typ=1;break;}

										if(komputera.y==losj) break;
									}
									if(komputera.typ) break;
									if(komputera.x==losi) break;
								}

								if(komputera.typ) break;
								paczka.focus_nr++;
							}
							
						/*Pobieranie parametrów bicia*/
							if((!piony[paczka.focus_nr].typ&&czy_bicie(komputera.x, komputera.y, paczka.focus, srodki, piony))
								||(piony[paczka.focus_nr].typ&&dozwolona_dama(komputera.x, komputera.y, paczka.focus, piony, srodki)==2))
								{
									param_komputera=param_bicia(komputera.x, komputera.y, paczka.focus, srodki, piony);
									paczka.bylo_bicie=true;
								}

						/*Zmiana perspektywy (pozornego ustawienia planszy) i faktyczny ruch komputera*/
							zmien_gracza(srodki, pozycje, piony, okno); gracz=!gracz;
							normalizuj(paczka.focus_nr, paczka.focus, komputera.x, komputera.y, srodki);
							ruch_komputera(komputera.x, komputera.y, paczka.focus, pozycje, srodki, piony, okno, stlo); Sleep(500);

							if(piony[paczka.focus_nr].y==pozycje[0][0].y)
								piony[paczka.focus_nr].typ=1;

							if(paczka.bylo_bicie)
							{
								bicie_maszyny(param_komputera.x, param_komputera.y, srodki, piony);
								if(koniec()){ Sleep(500); return 0; }

								zmien_gracza(srodki, pozycje, piony, okno); gracz=!gracz;
								normalizuj(paczka.focus_nr, paczka.focus, komputera.x, komputera.y, srodki);

								if(!jakiekolwiek_bicie(paczka.focus, srodki, piony))
									{ zmien_gracza(srodki, pozycje, piony, okno); gracz=!gracz; break; }
							}
							else break;
						}
						
						paczka.bylo_bicie=false;
					}
				}

		/*Opcja zmiany gracza, w trybie testowym*/
			if(!tryb) if(zdarzenie.type==sf::Event::MouseButtonPressed&&zdarzenie.mouseButton.button==sf::Mouse::Right)
				{ zmien_gracza(srodki, pozycje, piony, okno); gracz=!gracz; }
		}
		wyrysuj();
	}
	return 0;
}

/*Wype³nianie tablic srodki i pozycje wartoœciami domyœlnymi*/
void tablice()
{
	int *skladnik=new int;
	for(int i=0; i<10; i++)
	{
		*skladnik=4+r_pola/2;
		if(i%2) *skladnik+=r_pola;
		for(int j=0; j<5; j++)
		{
			srodki[i][j].y=r_okna-r_pola*i-r_pola/2-3;

			srodki[i][j].x=(*skladnik);
			*skladnik+=r_pola*2;

			pozycje[i][j].y=srodki[i][j].y-r_piona;
			pozycje[i][j].x=srodki[i][j].x-r_piona;

			srodki[i][j].typ=pozycje[i][j].typ=-1;
			srodki[i][j].gracz=pozycje[i][j].gracz=0;
		}
	}
	delete skladnik;
}

/*Rysowanie pionów, t³a oraz samego okna*/
void wyrysuj()
{
	okno.clear(); okno.draw(stlo);
	rysuj_obiekty(piony, okno);
	okno.display();
}

bool koniec()
{
	for(unsigned int i=0; i<piony.size()-1; i++)
		if(piony[i].gracz!=piony[i+1].gracz) return false;

	string s;
	if(piony[0].gracz) s="Zwyciê¿aj¹ czarne"; else s="Zwyciê¿aj¹ bia³e";

	MessageBox(NULL, s.c_str(), "Gratulacje!", 0);
	return true;
}