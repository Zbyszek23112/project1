#include "Explosion.h"

#include <iostream>

Explosion::Explosion(sf::Vector2<float> pos, sf::Texture* texptr):
	Object(pos),
	AnimEnded(false),
	Frames(0),
	AnimInterval(0.0f),
	CurrentFrame(0),
	AnimTime(0.0f),
	FrameSizeX(0),
	FrameSizeY(0),
	TexPtr(texptr),
	Tex(nullptr)
{
	//Ustaw teksturê i sprite oraz parametry animacji
	switch (rand() % 3)
	{
	case 0:
		Tex = &TexPtr[0];
		Frames = 20;
		FrameSizeX = 50;
		FrameSizeY = 50;
		AnimInterval = 0.05f;
		break;
	case 1:
		Tex = &TexPtr[1];
		Frames = 64;
		FrameSizeX = 192;
		FrameSizeY = 192;
		AnimInterval = 0.05f;
		break;
	case 2:
		Tex = &TexPtr[2];
		Frames = 48;
		FrameSizeX = 256;
		FrameSizeY = 256;
		AnimInterval = 0.05f;
		break;
	default:
		std::cout << "Explosion: wrong texture type!" << std::endl;
		break;
	}
	


	Spr.setTexture(*Tex);
	Spr.setTextureRect(sf::IntRect(0, 0, FrameSizeX, FrameSizeY));

	//Ustaw origin point na œrodek do obrotów
	Spr.setOrigin(Spr.getLocalBounds().width / 2.0f, Spr.getLocalBounds().height / 2.0f);

	//Ustaw pozycjê
	Spr.setPosition(Pos);
}
Explosion::~Explosion()
{
}
sf::Sprite& Explosion::GetSprite(sf::Time elapsed)
{
	//Uzyskaj ró¿niczkê czasu pomiêdzy klatkami do uniezale¿nienia animacji od iloœci klatek na sekundê
	float dt = (float)elapsed.asMicroseconds() / 1000000.0f;

	//Animuj
	AnimTime += dt;
	if (AnimTime > AnimInterval)
	{
		if (CurrentFrame < Frames)
		{
			//Zmiana klatki
			++CurrentFrame;
			AnimTime = 0.0f;

			Spr.setTextureRect(sf::IntRect(CurrentFrame * FrameSizeX, 0, FrameSizeX, FrameSizeY));
		}
		else AnimEnded = true;
	}
	

	return Spr;
}
bool Explosion::GetAnimEnded()
{
	return AnimEnded;
}