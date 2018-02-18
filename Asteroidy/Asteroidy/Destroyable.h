#pragma once

#include "Object.h"

//Obiekt, kt�ry mo�e by� dynamicznie niszczony, np. asteroida, pocisk
class Destroyable : public Object
{
protected:

	float DistTraveled; //pokonany dystans
	float Range; //odleg�o�� po kt�rej obiekt jest niszczony
	float Radius; //promie� obiektu
	bool ShouldBeDestroyed; //Czy nale�y zniszczy� obiekt? Np. na skutek kolizji z innym obiektem

public:

	Destroyable(sf::Vector2<float> pos);
	~Destroyable();

	//Sprawd� przebyty dystans i zwr�� info o konieczno�ci zniszczenia obiektu
	bool CheckDistanceTraveled();

	sf::Vector2<float> GetPos(); //pobierz pozycj�
	float GetRadius(); //pobierz promie�
	void SetShouldBeDestroyed(bool destroy); //ustal czy zniszczy� obiekt
};