#pragma once

#include "Object.h"

class Explosion : public Object
{
private:

	sf::Sprite Spr; //sprite eksplozji
	sf::Texture* TexPtr; //WskaŸnik na tekstury, bo na wczytanie realtime s¹ za du¿e
	sf::Texture* Tex; //WskaŸnik na wybran¹ teksturê


	bool AnimEnded; //Czy animacja siê skoñczy³a i mo¿na usun¹æ eksplozjê?
	int Frames; //iloœæ klatek animacji
	float AnimInterval; //Interwa³ czasu do zmiany klatki animacji
	int CurrentFrame; //aktualna klatka animacji
	float AnimTime; //czas do zmiany klatki
	int FrameSizeX, FrameSizeY; //rozmiar klatek animacji

public:

	Explosion(sf::Vector2<float> pos, sf::Texture* texptr);
	~Explosion();

	//Metoda nakrywaj¹ca, zwraca odpowiedniego sprajta do animacji
	sf::Sprite& GetSprite(sf::Time elapsed) override;

	//SprawdŸ czy animacja siê skoñczy³a
	bool GetAnimEnded();
};