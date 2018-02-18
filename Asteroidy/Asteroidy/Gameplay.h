#pragma once

#include "Ship.h"
#include "Bullet.h"
#include "Asteroid.h"
#include "Explosion.h"
#include "Powerup.h"

#include <list>

class Gameplay
{
private:

	sf::RenderWindow Window; //okno SFML
	Ship* Spaceship; //obiekt statku gracza, wskaŸnik, bo bêdziemy go resetowaæ po spadku HP do zera
	std::list<Bullet*> BulletList; //lista pocisków statku do wyœwietlenia, u¿ywamy listy, gdy¿ pozwala ona
								   //na szybkie usuwanie elementów z dowolnego indeksu
	std::list<Asteroid*> AsteroidList; //lista z asteroidami
	std::list<Explosion*> ExplosionList; //lista z eksplozjami
	std::list<Powerup*> PowerupList; //lista z powerupami

	sf::Font Fnt; //czcionka

	float TimeToSpawnAsteroid; //czas odliczaj¹cy do zespawnowania kolejnej asteroidy

	sf::Texture BgrTex; //Tekstura t³a
	sf::Texture ExplosionTex[3]; //Tekstury eksplozji s¹ za du¿e do wczytywania w czasie rzeczywistym. Trzeba je gromadziæ tutaj.
	sf::Sprite BgrSpr; //Sprite t³a

public:

	Gameplay();
	~Gameplay();

	void Run(); //uruchom grê

	void Draw(sf::Time elapsed); //narysuj wszystkie obiekty na ekranie, pobierz czas pomiêdzy klatkami by uniezale¿niæ
							//prêdkoœæ animacji od liczby klatek na sekundê
};