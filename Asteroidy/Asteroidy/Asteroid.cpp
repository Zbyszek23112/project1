#include "Asteroid.h"

#include <iostream>

Asteroid::Asteroid(char* tex_path, char type, sf::Vector2<float> pos) :
	Destroyable(sf::Vector2<float>(0.0f, 0.0f)),
	VecForward(0.0f, -1.0f),
	Dir(0.0f),
	DirAnim(0.0f),
	AnimSpeed(100.0f),
	Speed(100.0f),
	HP(1),
	PointValue(0),
	Type(0)
{
	//Wczytaj tekstur� i sprite
	if (!Tex.loadFromFile(tex_path)) std::cout << "Cannot load: " + std::string(tex_path) << std::endl;

	Spr.setTexture(Tex);

	//Je�li nie sprecyzowano typu, wybierz losowo r�ne asteroidy z tekstury
	int CheckType = 0;
	if (type >= 6) CheckType = rand() % 6;
	else CheckType = type;
	switch (CheckType)
	{
	case 0:
		Spr.setTextureRect(sf::IntRect(0, 153, 53, 55));
		Radius = 29;
		HP = 5;
		PointValue = 20;
		Type = 0;
		break;
	case 1:
		Spr.setTextureRect(sf::IntRect(58, 167, 38, 38));
		Radius = 19;
		HP = 3;
		PointValue = 10;
		Type = 1;
		break;
	case 2:
		Spr.setTextureRect(sf::IntRect(104, 176, 12, 13));
		Radius = 6;
		HP = 1;
		PointValue = 5;
		Type = 2;
		break;
	case 3:
		Spr.setTextureRect(sf::IntRect(0, 211, 51, 49));
		Radius = 28;
		HP = 4;
		PointValue = 15;
		Type = 3;
		break;
	case 4:
		Spr.setTextureRect(sf::IntRect(53, 219, 30, 30));
		Radius = 16;
		HP = 2;
		PointValue = 10;
		Type = 4;
		break;
	case 5:
		Spr.setTextureRect(sf::IntRect(89, 221, 24, 24));
		Radius = 14;
		HP = 1;
		PointValue = 5;
		Type = 5;
		break;
	default:
		std::cout << "Asteroid: wrong sprite number!" << std::endl;
	}

	//Ustaw origin point na �rodek do obrot�w
	Spr.setOrigin(Spr.getLocalBounds().width / 2.0f, Spr.getLocalBounds().height / 2.0f);

	//Ustaw losowe parametry
	DirAnim = -180.0f + (float)(rand() % 360); //pocz�tkowy obr�t animacji
	Dir = -180.0f + (float)(rand() % 360); //pocz�tkowy obr�t
	AnimSpeed = -300.0f + (float)(rand() % 600); //pr�dko�� animacji obrotu
	Speed = 50.0f + (float)(rand() % 200); //pr�dko�� ruchu asteroidy

	//Obr�� wektor wyznaczaj�cy tor lotu pocisku
	sf::Transform AsteroTransform;
	AsteroTransform.rotate(Dir);
	VecForward = AsteroTransform * VecForward;

	//Przesu� asteroid� od �rodka do kraw�dzi ekranu,
	//dodaj losow� warto�� do pozycji �rodka, �eby nie wszystkie asteroidy celowa�y w �rodek ekranu.
	//Je�li zdefiniowano typ, to pozycja nie jest losowa.
	if (type >= 6)
	{
		Pos = sf::Vector2<float>(100.0f + (float)(rand() % 600), 100.0f + (float)(rand() % 500));
		Pos -= VecForward * 1000.0f;
	}
	else Pos = pos;


	//Przekr�� asteroid�
	Spr.setRotation(DirAnim);
}
Asteroid::~Asteroid()
{
}
sf::Sprite& Asteroid::GetSprite(sf::Time elapsed)
{
	//Uzyskaj r�niczk� czasu pomi�dzy klatkami do uniezale�nienia animacji od ilo�ci klatek na sekund�
	float dt = (float)elapsed.asMicroseconds() / 1000000.0f;

	//Animuj asteroid�
	DirAnim += AnimSpeed * dt;
	if (DirAnim > 180.0f) DirAnim -= 360.0f;
	else if (DirAnim < -180.0f) DirAnim += 360.0f;
	Spr.setRotation(DirAnim);

	//Przemieszczaj asteroid�
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
int Asteroid::GetHP()
{
	return HP;
}
void Asteroid::RemoveHP(int how_many)
{
	HP -= how_many;
	if (HP < 0) HP = 0;

	return;
}
int Asteroid::GetValue()
{
	return PointValue;
}
char Asteroid::GetType()
{
	return Type;
}