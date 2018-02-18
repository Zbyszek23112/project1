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
	//Wczytaj teksturê i sprite
	if (!Tex.loadFromFile(tex_path)) std::cout << "Cannot load: " + std::string(tex_path) << std::endl;

	Spr.setTexture(Tex);

	//Jeœli nie sprecyzowano typu, wybierz losowo ró¿ne asteroidy z tekstury
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

	//Ustaw origin point na œrodek do obrotów
	Spr.setOrigin(Spr.getLocalBounds().width / 2.0f, Spr.getLocalBounds().height / 2.0f);

	//Ustaw losowe parametry
	DirAnim = -180.0f + (float)(rand() % 360); //pocz¹tkowy obrót animacji
	Dir = -180.0f + (float)(rand() % 360); //pocz¹tkowy obrót
	AnimSpeed = -300.0f + (float)(rand() % 600); //prêdkoœæ animacji obrotu
	Speed = 50.0f + (float)(rand() % 200); //prêdkoœæ ruchu asteroidy

	//Obróæ wektor wyznaczaj¹cy tor lotu pocisku
	sf::Transform AsteroTransform;
	AsteroTransform.rotate(Dir);
	VecForward = AsteroTransform * VecForward;

	//Przesuñ asteroidê od œrodka do krawêdzi ekranu,
	//dodaj losow¹ wartoœæ do pozycji œrodka, ¿eby nie wszystkie asteroidy celowa³y w œrodek ekranu.
	//Jeœli zdefiniowano typ, to pozycja nie jest losowa.
	if (type >= 6)
	{
		Pos = sf::Vector2<float>(100.0f + (float)(rand() % 600), 100.0f + (float)(rand() % 500));
		Pos -= VecForward * 1000.0f;
	}
	else Pos = pos;


	//Przekrêæ asteroidê
	Spr.setRotation(DirAnim);
}
Asteroid::~Asteroid()
{
}
sf::Sprite& Asteroid::GetSprite(sf::Time elapsed)
{
	//Uzyskaj ró¿niczkê czasu pomiêdzy klatkami do uniezale¿nienia animacji od iloœci klatek na sekundê
	float dt = (float)elapsed.asMicroseconds() / 1000000.0f;

	//Animuj asteroidê
	DirAnim += AnimSpeed * dt;
	if (DirAnim > 180.0f) DirAnim -= 360.0f;
	else if (DirAnim < -180.0f) DirAnim += 360.0f;
	Spr.setRotation(DirAnim);

	//Przemieszczaj asteroidê
	float dx = VecForward.x * Speed * dt;
	float dy = VecForward.y * Speed * dt;
	Pos += sf::Vector2<float>(dx, dy);

	//Oblicz przebyty zasiêg
	float dist = sqrt(dx*dx + dy*dy);
	DistTraveled += dist;

	//Umieœæ aktualn¹ pozycjê w sprajcie:
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