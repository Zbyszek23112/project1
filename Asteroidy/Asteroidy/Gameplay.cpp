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
	
	//Wczytaj czcionkê
	if (!Fnt.loadFromFile("images/Raleway-Black.ttf")) std::cout << "Cannot load: " + std::string("images/Raleway-Black.ttf") << std::endl;

	//Zrób preload tekstur eksplozji
	if (!ExplosionTex[0].loadFromFile("images/explosions/type_A.png")) std::cout << "Cannot load: " + std::string("images/explosions/type_A.png") << std::endl;
	if (!ExplosionTex[1].loadFromFile("images/explosions/type_B.png")) std::cout << "Cannot load: " + std::string("images/explosions/type_B.png") << std::endl;
	if (!ExplosionTex[2].loadFromFile("images/explosions/type_C.png")) std::cout << "Cannot load: " + std::string("images/explosions/type_C.png") << std::endl;

	//Wczytaj teksturê i sprite t³a
	if (!BgrTex.loadFromFile("images/background.jpg")) std::cout << "Cannot load: " + std::string("images/background.jpg") << std::endl;
	BgrSpr.setTexture(BgrTex);

	//TESTOWA ASTEROIDA
	//Asteroid* A = new Asteroid("images/spaceship.png", sf::Vector2<float>(200.0f, 200.0f), 120.0f);
	//AsteroidList.push_back(A);
}
Gameplay::~Gameplay()
{
	//Sk³adnia for z C++11: iterowanie po ka¿dym elemencie listy
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
	sf::Clock clock; //zegar odmierzaj¹cy czas pomiêdzy klatkami

	while (Window.isOpen())
	{
		sf::Event event;
		while (Window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				Window.close();
		}

		Window.clear();

		//Narysuj t³o
		Window.draw(BgrSpr);

		sf::Time elapsed = clock.restart(); //pobierz czas jaki up³yn¹³ od ostatniej klatki

		//Uzyskaj ró¿niczkê czasu pomiêdzy klatkami do uniezale¿nienia animacji od iloœci klatek na sekundê
		float dt = (float)elapsed.asMicroseconds() / 1000000.0f;

		//Spawnuj asteroidy co okreœlony czas
		TimeToSpawnAsteroid += dt;
		if (TimeToSpawnAsteroid > 0.5f)
		{
			TimeToSpawnAsteroid = 0.0f;

			Asteroid* A = new Asteroid("images/spaceship.png");
			AsteroidList.push_back(A);
		}

		//iteruj po wszystkich elementach listy pocisków i sprawdŸ czy któryœ pocisk nie przekroczy³ swojego zasiêgu
		//sk³adnia for C++11
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

		//iteruj po wszystkich elementach listy asteroid i sprawdŸ czy któraœ nie wylecia³a za planszê
		//sk³adnia for C++11
		std::list<Asteroid*> AsteroidsToDestroy;
		for (Asteroid* A : AsteroidList)
			if (A->CheckDistanceTraveled()) AsteroidsToDestroy.push_back(A);
		
		for (Asteroid* A : AsteroidsToDestroy)
			if (A)
			{
				//Wytwórz eksplozjê na pozycji asteroidy
				Explosion* E = new Explosion(A->GetPos(), ExplosionTex);
				ExplosionList.push_back(E);

				delete A;
				AsteroidList.remove(A);
			}
		AsteroidsToDestroy.clear();

		//iteruj po wszystkich elementach listy powerupów i sprawdŸ czy któregoœ nie trzeba zniszczyæ
		//sk³adnia for C++11
		std::list<Powerup*> PowerupsToDestroy;
		for (Powerup* P : PowerupList)
			//Powerup siê nie porusza, ale jest tam sprawdzany parametr ShouldBeDestroyed.
			if (P->CheckDistanceTraveled()) PowerupsToDestroy.push_back(P); 

		for (Powerup* P : PowerupsToDestroy)
			if (P)
			{
				delete P;
				PowerupList.remove(P);
			}
		PowerupsToDestroy.clear();

		//iteruj po wszystkich elementach listy eksplozji i sprawdŸ czy któraœ nie zakoñczy³a animacji
		//sk³adnia for C++11
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


		//SprawdŸ HP statku i w razie koniecznoœci zresetuj go
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

		//Jeœli wciœniêto ESC, wyjdŸ z gry.
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) break;
	}

	return;
}
void Gameplay::Draw(sf::Time elapsed)
{
	Window.draw(Spaceship->GetSprite(elapsed)); //rysuj statek kosmiczny

	//iteruj po wszystkich elementach listy pocisków i narysuj wszystkie pociski z listy
	//sk³adnia for C++11
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