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
	Ship* Spaceship; //obiekt statku gracza, wska�nik, bo b�dziemy go resetowa� po spadku HP do zera
	std::list<Bullet*> BulletList; //lista pocisk�w statku do wy�wietlenia, u�ywamy listy, gdy� pozwala ona
								   //na szybkie usuwanie element�w z dowolnego indeksu
	std::list<Asteroid*> AsteroidList; //lista z asteroidami
	std::list<Explosion*> ExplosionList; //lista z eksplozjami
	std::list<Powerup*> PowerupList; //lista z powerupami

	sf::Font Fnt; //czcionka

	float TimeToSpawnAsteroid; //czas odliczaj�cy do zespawnowania kolejnej asteroidy

	sf::Texture BgrTex; //Tekstura t�a
	sf::Texture ExplosionTex[3]; //Tekstury eksplozji s� za du�e do wczytywania w czasie rzeczywistym. Trzeba je gromadzi� tutaj.
	sf::Sprite BgrSpr; //Sprite t�a

public:

	Gameplay();
	~Gameplay();

	void Run(); //uruchom gr�

	void Draw(sf::Time elapsed); //narysuj wszystkie obiekty na ekranie, pobierz czas pomi�dzy klatkami by uniezale�ni�
							//pr�dko�� animacji od liczby klatek na sekund�
};