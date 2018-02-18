#pragma once

#include "Destroyable.h"

class Asteroid : public Destroyable
{
private:

	sf::Sprite Spr; //sprite asteroidy
	float Dir; //kierunek lotu asteroidy
	float DirAnim; //obrót asteroidy do animacji
	float AnimSpeed; //szybkoœæ animacji
	float Speed; //prêdkoœæ asteroidy
	int HP; //hit points
	int PointValue; //wartoœæ asteroidy w punktach
	char Type; //typ asteroidy

	sf::Vector2<float> VecForward; //wektor do przodu

public:

	//tylko œcie¿ka, reszta bêdzie losowa, type = 6: losowo, jak typ =6 to pos te¿ jest losowe
	Asteroid(char* tex_path, char type = 6, sf::Vector2<float> pos = sf::Vector2<float>(0.0f, 0.0f));
	~Asteroid();

	//Metoda nakrywaj¹ca, zwraca odpowiedniego sprajta do animacji
	sf::Sprite& GetSprite(sf::Time elapsed) override;

	//Pobierz HP
	int GetHP();

	//Usuñ HP na skutek trafienia
	void RemoveHP(int how_many);

	//Pobierz wartoœæ punktow¹ asteroidy
	int GetValue();

	//Pobierz typ asteroidy
	char GetType();

};