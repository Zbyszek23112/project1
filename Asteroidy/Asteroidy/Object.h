#pragma once

#include <SFML/Graphics.hpp>

class Object
{
protected:

	sf::Texture Tex; //tekstura obiektu
	sf::Vector2<float> Pos; //pozycja obiektu na ekranie, wektor dw�ch element�w typu float

public:

	//pozycja x i y
	Object(sf::Vector2<float> pos);
	~Object();

	//Pobierz referencj� sprite'a. Funkcja prawdziwie wirtualna, gdy� implementuje j� klasa pochodna
	//i zwraca odpowiedniego sprajta w zale�no�ci od klatki animacji
	//parametr: czas jaki up�yn�� od ostatniej klatki
	virtual sf::Sprite& GetSprite(sf::Time elapsed) = 0;
};