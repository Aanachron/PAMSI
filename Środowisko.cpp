#include"Œrodowisko.h"
#include<algorithm>
#include<Windows.h>

using namespace std;

int pseudomenu(sf::Font czcionka, sf::Sprite stlo, sf::RenderWindow& okno)
{
	sf::Event zdarzenie; sf::RectangleShape przycisk;
	int mx, my;	float py[3];
	float px=r_okna/2-r_przycisku.x/2;
	py[0]=125; py[1]=225; py[2]=325;

	sf::Text txt; txt.setFont(czcionka); string s="";
	txt.setCharacterSize(20);
	txt.setFillColor(sf::Color::Black);

	przycisk.setSize(r_przycisku);
	przycisk.setFillColor(sf::Color(192, 192, 192));
	przycisk.setOutlineColor(sf::Color::Black);
	przycisk.setOutlineThickness(1);

	while(okno.isOpen())
	{
		while(okno.pollEvent(zdarzenie))
		{
			if(zdarzenie.type==sf::Event::Closed) okno.close();
			if(zdarzenie.type==sf::Event::KeyPressed&&zdarzenie.key.code==sf::Keyboard::Escape) okno.close();
			if(zdarzenie.type==sf::Event::MouseButtonPressed&&zdarzenie.mouseButton.button==sf::Mouse::Left)
			{
			/*Odczytywanie pozycji i sprawdzanie, czy zosta³ wciœniêty przycisk*/
				mx=sf::Mouse::getPosition(okno).x; my=sf::Mouse::getPosition(okno).y;
				if(mx>px&&mx<px+r_przycisku.x)
				{
					if(my>py[0]&&my<py[0]+r_przycisku.y) return 1; /*0-testowy; 1-PvP; 2-komputer*/
					if(my>py[1]&&my<py[1]+r_przycisku.y) return 2;
					if(my>py[2]&&my<py[2]+r_przycisku.y) return 0;
				}
			}
		}

		okno.clear(); okno.draw(stlo);
		for(int i=0; i<3; i++)
		{
			przycisk.setPosition(px, py[i]); okno.draw(przycisk);

			if(!i) s="Tryb PvP"; else if(i==1) s="Komputer"; else s="Tryb testowy";
			txt.setPosition(przycisk.getPosition().x+r_przycisku.x/2-s.length()*5, przycisk.getPosition().y+12);
			txt.setString(s); okno.draw(txt);
		}
		okno.display();
	}

	return 0;
}

void rysuj_obiekty(Tobiekty obsy, sf::RenderWindow& okno)
{
	sf::CircleShape kolo; kolo.setRadius(r_piona);
	kolo.setOutlineColor(sf::Color(192, 192, 192));

	for(Tobiekty::const_iterator i=obsy.begin(); i!=obsy.end(); ++i)
	{
		kolo.setPosition((float)i->x, (float)i->y);
		if(i->gracz) kolo.setFillColor(sf::Color::Black);
		else kolo.setFillColor(sf::Color::White);
		if(i->typ) kolo.setOutlineThickness(4);

		okno.draw(kolo);
		kolo.setOutlineThickness(0);
	}
}

void poz_wyjsciowe(koordy pozycje[10][5], koordy srodki[10][5], Tobiekty &piony)
{
	piony.clear();
	for(int i=0; i<10; i++) 
		for(int j=0; j<5; j++) 
		{
			if(i!=4&&i!=5) 
			{
				piony.push_back(koordy(pozycje[i][j].x, pozycje[i][j].y));
				srodki[i][j].typ=piony.size()-1;
			}
			if(i>5){ piony.back().gracz=1; srodki[i][j].gracz=1;}
		}
}

void poz_testowe(koordy pozycje[10][5], koordy srodki[10][5], Tobiekty &piony)
{
	piony.clear();

	for(int i=0; i<5; i++)
	{
		piony.push_back(koordy(pozycje[3][i].x, pozycje[3][i].y));
		piony.back().gracz=0; srodki[3][i].typ=piony.size()-1; srodki[3][i].gracz=0;

		piony.push_back(koordy(pozycje[6][i].x, pozycje[6][i].y));
		piony.back().gracz=1; srodki[6][i].typ=piony.size()-1; srodki[6][i].gracz=1;
	}
}

void zmien_gracza(koordy srodki[10][5], koordy pozycje[10][5], Tobiekty &piony, sf::RenderWindow &okno)
{
	koordy tmp[10][5];

	for(int i=0; i<10; i++)
		for(int j=0; j<5; j++)
			{ tmp[i][j].typ=srodki[i][j].typ; tmp[i][j].gracz=srodki[i][j].gracz;}

	for(int i=0; i<10; i++)
		for(int j=0; j<5; j++)
		{
			srodki[9-i][4-j].typ=tmp[i][j].typ;
			srodki[9-i][4-j].gracz=tmp[i][j].gracz;
			if(tmp[i][j].typ!=-1) 
			{ 
				piony[tmp[i][j].typ].x=pozycje[9-i][4-j].x;
				piony[tmp[i][j].typ].y=pozycje[9-i][4-j].y;
			}
		}
}
