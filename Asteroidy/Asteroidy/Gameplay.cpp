#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

#include "Gameplay.h"

Gameplay::Gameplay():
	Window(sf::VideoMode(800, 600), "Asteroidy"),
	Spaceship(new Ship("images/spaceship.png", sf::Vector2<float>(400.0f, 300.0f), BulletList, AsteroidList, PowerupList)),
	TimeToSpawnAsteroid(0.0f)
{
	srand((unsigned int)time(nullptr)); //ustaw ziarno generatora liczb pseudolosowych na aktualny czas
	
	//Wczytaj czcionk�
	if (!Fnt.loadFromFile("images/Raleway-Black.ttf")) std::cout << "Cannot load: " + std::string("images/Raleway-Black.ttf") << std::endl;

	//Zr�b preload tekstur eksplozji
	if (!ExplosionTex[0].loadFromFile("images/explosions/type_A.png")) std::cout << "Cannot load: " + std::string("images/explosions/type_A.png") << std::endl;
	if (!ExplosionTex[1].loadFromFile("images/explosions/type_B.png")) std::cout << "Cannot load: " + std::string("images/explosions/type_B.png") << std::endl;
	if (!ExplosionTex[2].loadFromFile("images/explosions/type_C.png")) std::cout << "Cannot load: " + std::string("images/explosions/type_C.png") << std::endl;

	//Wczytaj tekstur� i sprite t�a
	if (!BgrTex.loadFromFile("images/background.jpg")) std::cout << "Cannot load: " + std::string("images/background.jpg") << std::endl;
	BgrSpr.setTexture(BgrTex);

	//TESTOWA ASTEROIDA
	//Asteroid* A = new Asteroid("images/spaceship.png", sf::Vector2<float>(200.0f, 200.0f), 120.0f);
	//AsteroidList.push_back(A);
}
Gameplay::~Gameplay()
{
	//Sk�adnia for z C++11: iterowanie po ka�dym elemencie listy
	for (Bullet* B : BulletList)
		if (B)
		{
			delete B;
		}

	for (Asteroid* A : AsteroidList)
		if (A)
		{
			delete A;
		}

	for (Explosion* E : ExplosionList)
		if (E)
		{
			delete E;
		}

	for (Powerup* P : PowerupList)
		if (P)
		{
			delete P;
		}

	if (Spaceship) delete Spaceship;
}
void Gameplay::Run()
{
	std::cout << "Running..." << std::endl;
	sf::Clock clock; //zegar odmierzaj�cy czas pomi�dzy klatkami

	while (Window.isOpen())
	{
		sf::Event event;
		while (Window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				Window.close();
		}

		Window.clear();

		//Narysuj t�o
		Window.draw(BgrSpr);

		sf::Time elapsed = clock.restart(); //pobierz czas jaki up�yn�� od ostatniej klatki

		//Uzyskaj r�niczk� czasu pomi�dzy klatkami do uniezale�nienia animacji od ilo�ci klatek na sekund�
		float dt = (float)elapsed.asMicroseconds() / 1000000.0f;

		//Spawnuj asteroidy co okre�lony czas
		TimeToSpawnAsteroid += dt;
		if (TimeToSpawnAsteroid > 0.5f)
		{
			TimeToSpawnAsteroid = 0.0f;

			Asteroid* A = new Asteroid("images/spaceship.png");
			AsteroidList.push_back(A);
		}

		//iteruj po wszystkich elementach listy pocisk�w i sprawd� czy kt�ry� pocisk nie przekroczy� swojego zasi�gu
		//sk�adnia for C++11
		std::list<Bullet*> BulletsToDestroy;
		for (Bullet* B : BulletList)
			if (B->CheckDistanceTraveled()) BulletsToDestroy.push_back(B);
		for (Bullet* B : BulletsToDestroy)
			if (B)
			{
				delete B;
				BulletList.remove(B);
			}
		BulletsToDestroy.clear();

		//iteruj po wszystkich elementach listy asteroid i sprawd� czy kt�ra� nie wylecia�a za plansz�
		//sk�adnia for C++11
		std::list<Asteroid*> AsteroidsToDestroy;
		for (Asteroid* A : AsteroidList)
			if (A->CheckDistanceTraveled()) AsteroidsToDestroy.push_back(A);
		
		for (Asteroid* A : AsteroidsToDestroy)
			if (A)
			{
				//Wytw�rz eksplozj� na pozycji asteroidy
				Explosion* E = new Explosion(A->GetPos(), ExplosionTex);
				ExplosionList.push_back(E);

				delete A;
				AsteroidList.remove(A);
			}
		AsteroidsToDestroy.clear();

		//iteruj po wszystkich elementach listy powerup�w i sprawd� czy kt�rego� nie trzeba zniszczy�
		//sk�adnia for C++11
		std::list<Powerup*> PowerupsToDestroy;
		for (Powerup* P : PowerupList)
			//Powerup si� nie porusza, ale jest tam sprawdzany parametr ShouldBeDestroyed.
			if (P->CheckDistanceTraveled()) PowerupsToDestroy.push_back(P); 

		for (Powerup* P : PowerupsToDestroy)
			if (P)
			{
				delete P;
				PowerupList.remove(P);
			}
		PowerupsToDestroy.clear();

		//iteruj po wszystkich elementach listy eksplozji i sprawd� czy kt�ra� nie zako�czy�a animacji
		//sk�adnia for C++11
		std::list<Explosion*> ExplosionsToDestroy;
		for (Explosion* E : ExplosionList)
			if (E->GetAnimEnded()) ExplosionsToDestroy.push_back(E);

		for (Explosion* E : ExplosionsToDestroy)
			if (E)
			{
				delete E;
				ExplosionList.remove(E);
			}
		ExplosionsToDestroy.clear();


		//Sprawd� HP statku i w razie konieczno�ci zresetuj go
		if (Spaceship->GetHP() == 0)
		{
			delete Spaceship;
			Spaceship = new Ship("images/spaceship.png", sf::Vector2<float>(400.0f, 300.0f), BulletList, AsteroidList, PowerupList);

			std::cout << "Ship HP zero! Resetting spaceship..." << std::endl;
		}


		Draw(elapsed); //narysuj wszystko

		//Wypisz HP statku
		sf::Text t(std::string("Spaceship health: ") + std::to_string(Spaceship->GetHP()), Fnt);
		t.setCharacterSize(14);
		t.setPosition(sf::Vector2<float>(10.0f, 10.0f));
		Window.draw(t);

		//Wypisz punkty
		t.setString(std::string("Points: ") + std::to_string(Spaceship->GetPoints()));
		t.setPosition(sf::Vector2<float>(10.0f, 30.0f));
		Window.draw(t);

		Window.display();

		//Je�li wci�ni�to ESC, wyjd� z gry.
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) break;
	}

	return;
}
void Gameplay::Draw(sf::Time elapsed)
{
	Window.draw(Spaceship->GetSprite(elapsed)); //rysuj statek kosmiczny

	//iteruj po wszystkich elementach listy pocisk�w i narysuj wszystkie pociski z listy
	//sk�adnia for C++11
	for (Bullet* B : BulletList)
		Window.draw(B->GetSprite(elapsed));

	//rysuj asteroidy
	for (Asteroid* A : AsteroidList)
		Window.draw(A->GetSprite(elapsed));

	//rysuj powerupy
	for (Powerup* P : PowerupList)
		Window.draw(P->GetSprite(elapsed));

	//rysuj eksplozje
	for (Explosion* E : ExplosionList)
		Window.draw(E->GetSprite(elapsed));

	return;
}