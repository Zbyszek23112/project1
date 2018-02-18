#pragma once

#include "Object.h"
#include "Asteroid.h"
#include "Bullet.h"
#include "Powerup.h"

#include <list>

class Ship : public Object
{
private:

	//typ klatki animacji do odczytania z tablicy Spr
	enum SpriteType
	{
		LeftNoThrust = 0,
		ForwardNoThrust,
		RightNoThrust,
		LeftThrust1,
		ForwardThrust1,
		RightThrust1,
		LeftThrust2,
		ForwardThrust2,
		RightThrust2
	};
	SpriteType MoveState; //stan ruchu statku

	sf::Sprite Spr[9]; //sprites statku
	std::list<Bullet*>& Bullets; //Lista wystrzelonych pocisk�w, obs�ugiwana przez Gameplay
	std::list<Asteroid*>& Asteroids; //Lista asteroid w przestrzeni
	std::list<Powerup*>& Powerups; //Lista powerup�w w przestrzeni
	
	float FlameAnimTime; //czas u�ywany przy animacji p�omienia
	bool FlameNum; //numer klatki z p�omieniem do animacji (0 albo 1)
	float NextBulletTime; //czas jaki pozosta� do mo�liwo�ci wystrzelenia nast�pnego pocisku
	float Dir; //k�t obrotu statku
	float Speed; //pr�dko�� statku
	int HP; //hit points
	int Points; //punkty za zniszczenie asteroid
	float BulletTimeInterval; //odst�p czasu do nast�pnego pocisku

	sf::Vector2<float> DefaultVecForward, DefaultVecRight; //domy�lny wektor wyznaczaj�cy zwrot do przodu i w prawo
	sf::Vector2<float> VecForward, VecRight; //aktualny wektor wyznaczaj�cy zwrot do przodu i w prawo

	//Przetw�rz input od gracza
	void CheckInput(float dt);

	//Sprawd� kolizj� z asteroidami
	void CheckCollision();

	//Za�aduj plik konfiguracyjny
	void LoadConfigFile();

public:

	//�cie�ka do tekstury, pozycja x i y, lista na wystrzelone pociski
	Ship(char* tex_path, sf::Vector2<float> pos, std::list<Bullet*>& bullets, std::list<Asteroid*>& asteroids, std::list<Powerup*>& powerups);
	~Ship();

	//Metoda nakrywaj�ca, zwraca odpowiedniego sprajta do animacji
	sf::Sprite& GetSprite(sf::Time elapsed) override;

	//Pobierz punkty �ycia statku
	int GetHP();

	//pobierz punkty
	int GetPoints(); 

	//dodaj punkty
	void AddPoints(int how_many);

};