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
	float Speed; //prêdkoœæ
	float Dir; //kierunek

	sf::Vector2<float> VecForward; //Wektor równoleg³y do toru lotu pocisku

public:

	//referencja listy z pociskami, œcie¿ka do tekstury, pozycja, k¹t, prêdkoœæ, moc, zasiêg
	Bullet(std::list<Bullet*>& bullets, char* tex_path, sf::Vector2<float> pos, float dir = 0.0f, float speed = 500.0f, float power = 1.0f, float range = 800.0f);
	~Bullet();

	//Metoda nakrywaj¹ca, zwraca odpowiedniego sprajta do animacji
	sf::Sprite& GetSprite(sf::Time elapsed) override;

	//Pobierz moc pocisku
	float GetPower();

};