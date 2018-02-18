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
	std::list<Bullet*>& Bullets; //Lista wystrzelonych pocisków, obs³ugiwana przez Gameplay
	std::list<Asteroid*>& Asteroids; //Lista asteroid w przestrzeni
	std::list<Powerup*>& Powerups; //Lista powerupów w przestrzeni
	
	float FlameAnimTime; //czas u¿ywany przy animacji p³omienia
	bool FlameNum; //numer klatki z p³omieniem do animacji (0 albo 1)
	float NextBulletTime; //czas jaki pozosta³ do mo¿liwoœci wystrzelenia nastêpnego pocisku
	float Dir; //k¹t obrotu statku
	float Speed; //prêdkoœæ statku
	int HP; //hit points
	int Points; //punkty za zniszczenie asteroid
	float BulletTimeInterval; //odstêp czasu do nastêpnego pocisku

	sf::Vector2<float> DefaultVecForward, DefaultVecRight; //domyœlny wektor wyznaczaj¹cy zwrot do przodu i w prawo
	sf::Vector2<float> VecForward, VecRight; //aktualny wektor wyznaczaj¹cy zwrot do przodu i w prawo

	//Przetwórz input od gracza
	void CheckInput(float dt);

	//SprawdŸ kolizjê z asteroidami
	void CheckCollision();

	//Za³aduj plik konfiguracyjny
	void LoadConfigFile();

public:

	//œcie¿ka do tekstury, pozycja x i y, lista na wystrzelone pociski
	Ship(char* tex_path, sf::Vector2<float> pos, std::list<Bullet*>& bullets, std::list<Asteroid*>& asteroids, std::list<Powerup*>& powerups);
	~Ship();

	//Metoda nakrywaj¹ca, zwraca odpowiedniego sprajta do animacji
	sf::Sprite& GetSprite(sf::Time elapsed) override;

	//Pobierz punkty ¿ycia statku
	int GetHP();

	//pobierz punkty
	int GetPoints(); 

	//dodaj punkty
	void AddPoints(int how_many);

};