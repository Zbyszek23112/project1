#pragma once

#include "Destroyable.h"

#include <list>

//Pocisk wystrzeliwany przez statek
class Bullet : public Destroyable
{
protected:

	sf::Sprite Spr; //sprite pocisku

	std::list<Bullet*>& Bullets; //referencja listy z pociskami

	float Power; //moc pocisku
	float Speed; //pr�dko��
	float Dir; //kierunek

	sf::Vector2<float> VecForward; //Wektor r�wnoleg�y do toru lotu pocisku

public:

	//referencja listy z pociskami, �cie�ka do tekstury, pozycja, k�t, pr�dko��, moc, zasi�g
	Bullet(std::list<Bullet*>& bullets, char* tex_path, sf::Vector2<float> pos, float dir = 0.0f, float speed = 500.0f, float power = 1.0f, float range = 800.0f);
	~Bullet();

	//Metoda nakrywaj�ca, zwraca odpowiedniego sprajta do animacji
	sf::Sprite& GetSprite(sf::Time elapsed) override;

	//Pobierz moc pocisku
	float GetPower();

};