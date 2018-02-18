#include "Bullet.h"

#include <iostream>

Bullet::Bullet(std::list<Bullet*>& bullets, char* tex_path, sf::Vector2<float> pos, float dir, float speed, float power, float range):
	Destroyable(pos),
	Bullets(bullets),
	Power(power),
	Speed(speed),
	Dir(dir),
	VecForward(0.0f, -1.0f)
{

	//Wczytaj tekstur� i sprite
	if (!Tex.loadFromFile(tex_path)) std::cout << "Cannot load: " + std::string(tex_path) << std::endl;

	Spr.setTexture(Tex);
	Spr.setTextureRect(sf::IntRect(2, 131, 7, 9));

	//Ustaw origin point na �rodek do obrot�w
	Spr.setOrigin(Spr.getLocalBounds().width / 2.0f, Spr.getLocalBounds().height / 2.0f);

	//Przekr�� pocisk
	Spr.setRotation(Dir);

	//Obr�� wektor wyznaczaj�cy tor lotu pocisku
	sf::Transform BulletTransform;
	BulletTransform.rotate(Dir);
	VecForward = BulletTransform * VecForward;
}
Bullet::~Bullet()
{
}

sf::Sprite& Bullet::GetSprite(sf::Time elapsed)
{
	//Uzyskaj r�niczk� czasu pomi�dzy klatkami do uniezale�nienia animacji od ilo�ci klatek na sekund�
	float dt = (float)elapsed.asMicroseconds() / 1000000.0f;

	//Przemie�� pocisk
	float dx = VecForward.x * Speed * dt;
	float dy = VecForward.y * Speed * dt;
	Pos += sf::Vector2<float>(dx, dy);

	//Oblicz przebyty zasi�g
	float dist = sqrt(dx*dx + dy*dy);
	DistTraveled += dist;

	//Umie�� aktualn� pozycj� w sprajcie:
	Spr.setPosition(Pos);

	return Spr;
}
float Bullet::GetPower()
{
	return Power;
}
