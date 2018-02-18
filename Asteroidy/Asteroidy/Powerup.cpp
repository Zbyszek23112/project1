#include "Powerup.h"

#include <iostream>

Powerup::Powerup(sf::Vector2<float> pos):
	Destroyable(pos)
{
	Radius = 5.0f;
	Range = 10.0f; //u�ywamy pola range jako maksymalny czas �ycia powerupa

	//Wczytaj tekstur� i sprite
	if (!Tex.loadFromFile("images/powerup.png")) std::cout << "Cannot load: " + std::string("images/powerup.png") << std::endl;
	Spr.setTexture(Tex);

	//Wylosuj typ powerupu
	Type = rand() % 2;
	Spr.setTextureRect(sf::IntRect(Type * 32, 0, 32, 32));

	//Ustaw origin point na �rodek
	Spr.setOrigin(Spr.getLocalBounds().width / 2.0f, Spr.getLocalBounds().height / 2.0f);
}
Powerup::~Powerup()
{
}
sf::Sprite& Powerup::GetSprite(sf::Time elapsed)
{
	//Uzyskaj r�niczk� czasu pomi�dzy klatkami
	float dt = (float)elapsed.asMicroseconds() / 1000000.0f;

	//U�yj pola DistTraveled do mierzenia czasu �ycia powerupa
	DistTraveled += dt;

	//Ustaw aktualn� pozycj�
	Spr.setPosition(Pos);

	return Spr;
}
int Powerup::GetType()
{
	return Type;
}