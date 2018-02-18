#include "Destroyable.h"

#include <iostream>

Destroyable::Destroyable(sf::Vector2<float> pos):
	Object(pos),
	DistTraveled(0.0f),
	Range(2000.0f),
	Radius(1.0f),
	ShouldBeDestroyed(false)
{
}
Destroyable::~Destroyable()
{
}
bool Destroyable::CheckDistanceTraveled()
{
	//Je�li dystans wi�kszy ni� zasi�g, zniszcz pocisk.
	if (DistTraveled > Range)
	{
		std::cout << "Object destroyed: max range reached." << std::endl;
		return true;
	}

	//Je�li ustawiono konieczno�� zniszczenia
	if (ShouldBeDestroyed)
	{
		std::cout << "Object destroyed: collision detected." << std::endl;
		return true;
	}

	return false;
}
sf::Vector2<float> Destroyable::GetPos()
{
	return Pos;
}
float Destroyable::GetRadius()
{
	return Radius;
}
void Destroyable::SetShouldBeDestroyed(bool destroy)
{
	ShouldBeDestroyed = destroy;
	return;
}