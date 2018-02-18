#pragma once

#include "Destroyable.h"

//Element do zebrania przez statek
class Powerup : public Destroyable
{
protected:

	sf::Sprite Spr; //sprajt powerupa
	int Type; //typo powerupu

public:

	//typy powerupów
	enum TypeEnum
	{
		HealthUp = 0,
		BulletSpeedUp
	};

	Powerup(sf::Vector2<float> pos);
	~Powerup();

	//Metoda nakrywaj¹ca, zwraca odpowiedniego sprajta do animacji
	sf::Sprite& GetSprite(sf::Time elapsed) override;

	//Pobierz typ powerupa
	int GetType();
};