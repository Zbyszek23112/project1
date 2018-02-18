#pragma once

#include "Object.h"

class Explosion : public Object
{
private:

	sf::Sprite Spr; //sprite eksplozji
	sf::Texture* TexPtr; //Wska�nik na tekstury, bo na wczytanie realtime s� za du�e
	sf::Texture* Tex; //Wska�nik na wybran� tekstur�


	bool AnimEnded; //Czy animacja si� sko�czy�a i mo�na usun�� eksplozj�?
	int Frames; //ilo�� klatek animacji
	float AnimInterval; //Interwa� czasu do zmiany klatki animacji
	int CurrentFrame; //aktualna klatka animacji
	float AnimTime; //czas do zmiany klatki
	int FrameSizeX, FrameSizeY; //rozmiar klatek animacji

public:

	Explosion(sf::Vector2<float> pos, sf::Texture* texptr);
	~Explosion();

	//Metoda nakrywaj�ca, zwraca odpowiedniego sprajta do animacji
	sf::Sprite& GetSprite(sf::Time elapsed) override;

	//Sprawd� czy animacja si� sko�czy�a
	bool GetAnimEnded();
};