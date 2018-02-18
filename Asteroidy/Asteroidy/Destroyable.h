#pragma once

#include "Object.h"

//Obiekt, który mo¿e byæ dynamicznie niszczony, np. asteroida, pocisk
class Destroyable : public Object
{
protected:

	float DistTraveled; //pokonany dystans
	float Range; //odleg³oœæ po której obiekt jest niszczony
	float Radius; //promieñ obiektu
	bool ShouldBeDestroyed; //Czy nale¿y zniszczyæ obiekt? Np. na skutek kolizji z innym obiektem

public:

	Destroyable(sf::Vector2<float> pos);
	~Destroyable();

	//SprawdŸ przebyty dystans i zwróæ info o koniecznoœci zniszczenia obiektu
	bool CheckDistanceTraveled();

	sf::Vector2<float> GetPos(); //pobierz pozycjê
	float GetRadius(); //pobierz promieñ
	void SetShouldBeDestroyed(bool destroy); //ustal czy zniszczyæ obiekt
};