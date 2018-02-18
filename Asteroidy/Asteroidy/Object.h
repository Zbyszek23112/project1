#pragma once

#include <SFML/Graphics.hpp>

class Object
{
protected:

	sf::Texture Tex; //tekstura obiektu
	sf::Vector2<float> Pos; //pozycja obiektu na ekranie, wektor dwóch elementów typu float

public:

	//pozycja x i y
	Object(sf::Vector2<float> pos);
	~Object();

	//Pobierz referencjê sprite'a. Funkcja prawdziwie wirtualna, gdy¿ implementuje j¹ klasa pochodna
	//i zwraca odpowiedniego sprajta w zale¿noœci od klatki animacji
	//parametr: czas jaki up³yn¹³ od ostatniej klatki
	virtual sf::Sprite& GetSprite(sf::Time elapsed) = 0;
};