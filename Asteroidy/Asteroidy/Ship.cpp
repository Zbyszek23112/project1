#include <iostream>
#include <fstream>

#include "Ship.h"

Ship::Ship(char* tex_path, sf::Vector2<float> pos, std::list<Bullet*>& bullets, std::list<Asteroid*>& asteroids, std::list<Powerup*>& powerups):
	Object(pos),
	MoveState(SpriteType::ForwardNoThrust),
	FlameAnimTime(0.0f),
	FlameNum(true),
	Bullets(bullets),
	Asteroids(asteroids),
	Powerups(powerups),
	NextBulletTime(0.0f),
	Dir(0.0f),
	Speed(200.0f),
	VecForward(0.0f, -1.0f),
	VecRight(1.0f, 0.0f),
	DefaultVecForward(0.0f, -1.0f),
	DefaultVecRight(1.0f, 0.0f),
	HP(3),
	Points(0),
	BulletTimeInterval(0.3f)
{
	//wczytaj teksturê, a jak siê nie uda - wyrzuæ b³¹d
	if (!Tex.loadFromFile(tex_path)) std::cout << "Cannot load: " + std::string(tex_path) << std::endl;
	
	//Stwórz klatki animacji z tekstury
	//no thrust
	Spr[SpriteType::LeftNoThrust].setTexture(Tex);
	Spr[SpriteType::LeftNoThrust].setTextureRect(sf::IntRect(0, 0, 36, 36));
	Spr[SpriteType::ForwardNoThrust].setTexture(Tex);
	Spr[SpriteType::ForwardNoThrust].setTextureRect(sf::IntRect(39, 0, 39, 36));
	Spr[SpriteType::RightNoThrust].setTexture(Tex);
	Spr[SpriteType::RightNoThrust].setTextureRect(sf::IntRect(80, 0, 36, 36));
	//thrust1
	Spr[SpriteType::LeftThrust1].setTexture(Tex);
	Spr[SpriteType::LeftThrust1].setTextureRect(sf::IntRect(0, 40, 36, 43));
	Spr[SpriteType::ForwardThrust1].setTexture(Tex);
	Spr[SpriteType::ForwardThrust1].setTextureRect(sf::IntRect(39, 40, 39, 43));
	Spr[SpriteType::RightThrust1].setTexture(Tex);
	Spr[SpriteType::RightThrust1].setTextureRect(sf::IntRect(80, 40, 36, 43));
	//thrust2
	Spr[SpriteType::LeftThrust2].setTexture(Tex);
	Spr[SpriteType::LeftThrust2].setTextureRect(sf::IntRect(0, 86, 36, 40));
	Spr[SpriteType::ForwardThrust2].setTexture(Tex);
	Spr[SpriteType::ForwardThrust2].setTextureRect(sf::IntRect(39, 86, 39, 40));
	Spr[SpriteType::RightThrust2].setTexture(Tex);
	Spr[SpriteType::RightThrust2].setTextureRect(sf::IntRect(80, 86, 36, 40));

	//Ustaw origin pointy na œrodek do obrotów, sk³adnia for C++11
	for(auto& S : Spr) S.setOrigin(S.getLocalBounds().width / 2.0f, S.getLocalBounds().height / 2.0f);

	LoadConfigFile();
}
Ship::~Ship()
{
}
sf::Sprite& Ship::GetSprite(sf::Time elapsed)
{

	//Uzyskaj ró¿niczkê czasu pomiêdzy klatkami do uniezale¿nienia animacji od iloœci klatek na sekundê
	float dt = (float)elapsed.asMicroseconds() / 1000000.0f;

	//Animuj p³omieñ z dysz statku
	FlameAnimTime += dt;
	if (FlameAnimTime > 0.2f)
	{
		FlameAnimTime = 0.0f;
		FlameNum = !FlameNum;
	}

	//Przetwórz input od gracza
	CheckInput(dt);
	
	//SprawdŸ kolizjê z asteroidami
	CheckCollision();

	//Umieœæ aktualn¹ transformacjê w aktualnym sprajcie:
	Spr[MoveState].setPosition(Pos);
	Spr[MoveState].setRotation(Dir);

	//Zwróæ sprajta w zale¿noœci od stanu ruchu
	return Spr[MoveState];
}
void Ship::CheckInput(float dt)
{
	//Wyznacz k¹t obrotu
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		Dir -= 200.0f * dt;
		if (Dir < -180.0f) Dir += 360.0f;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		Dir += 200.0f * dt;
		if (Dir > 180.0f) Dir -= 360.0f;
	}

	//Wyznacz nowy tor ruchu: transformuj zwroty do przodu i w prawo
	sf::Transform VecTransform;
	VecTransform.rotate(Dir);
	VecForward = VecTransform * DefaultVecForward;
	VecRight = VecTransform * DefaultVecRight;
	
	//SprawdŸ w któr¹ stronê porusza siê statek i wybierz odpowiedni¹ klatkê.
	//Obs³u¿ input z klawiatury
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		//przesuñ statek w lewo, prêdkoœæ w pikselach na sekundê
		Pos.x -= VecRight.x * Speed * dt;
		Pos.y -= VecRight.y * Speed * dt;

		MoveState = SpriteType::LeftNoThrust;

	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		//przesuñ statek w prawo, prêdkoœæ w pikselach na sekundê
		Pos.x += VecRight.x * Speed * dt;
		Pos.y += VecRight.y * Speed * dt;

		MoveState = SpriteType::RightNoThrust;

	}
	else MoveState = SpriteType::ForwardNoThrust;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		//przesuñ statek w górê, prêdkoœæ w pikselach na sekundê
		Pos.x += VecForward.x * Speed * dt;
		Pos.y += VecForward.y * Speed * dt;


		//Przy wciœniêtej strza³ce do góry statek emituje animowany p³omieñ z dysz
		if (MoveState == SpriteType::ForwardNoThrust)
		{
			MoveState = (FlameNum) ? SpriteType::ForwardThrust1 : SpriteType::ForwardThrust2;
		}
		else if (MoveState == SpriteType::LeftNoThrust)
		{
			MoveState = (FlameNum) ? SpriteType::LeftThrust1 : SpriteType::LeftThrust2;
		}
		else if (MoveState == SpriteType::RightNoThrust)
		{
			MoveState = (FlameNum) ? SpriteType::RightThrust1 : SpriteType::RightThrust2;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		//przesuñ statek w dó³, prêdkoœæ w pikselach na sekundê
		Pos.x -= VecForward.x * Speed * dt;
		Pos.y -= VecForward.y * Speed * dt;
	}

	//Obs³u¿ wylot poza ekran
	try
	{
		if (Pos.x > 830.0f) throw 1;
		else if (Pos.x < -30.0f) throw 2;

		if (Pos.y > 630.0f) throw 3;
		else if (Pos.y < -30.0f) throw 4;
	}
	catch (int exc)
	{
		switch (exc)
		{
		case 1:
			std::cout << "Ship exception: position x too high" << std::endl;
			Pos.x = -15.0f;
			break;
		case 2:
			std::cout << "Ship exception: position x too low" << std::endl;
			Pos.x = 815.0f;
			break;
		case 3:
			std::cout << "Ship exception: position y too high" << std::endl;
			Pos.y = -15.0f;
			break;
		case 4:
			std::cout << "Ship exception: position y too low" << std::endl;
			Pos.y = 615.0f;
			break;
		default:
			std::cout << "Unknown exception" << std::endl;
			break;
		}
	}

	//Obs³u¿ wystrzeliwanie pocisków
	NextBulletTime += dt;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && NextBulletTime > BulletTimeInterval)
	{
		NextBulletTime = 0.0f; //wyzeruj czas do nastêpnego pocisku, by zegar odlicza³ ponownie

							   //Transformuj punkt spawnu pocisku, by obraca³ siê razem ze statkiem
		sf::Transform BulletTransform;
		BulletTransform.rotate(Dir);
		sf::Vector2<float> BulletSpawnPointLocal = sf::Vector2<float>(0.0f, -15.0f);
		BulletSpawnPointLocal = BulletTransform * BulletSpawnPointLocal;

		//Stwórz nowy pocisk i dodaj go do listy pocisków do wyœwietlenia. Lista obs³ugiwana jest przez Gameplay.
		Bullet* B = new Bullet(Bullets, "images/spaceship.png", Pos + BulletSpawnPointLocal, Dir);
		Bullets.push_back(B);
	}

	return;
}
void Ship::CheckCollision()
{
	//Iteruj po wszystkich asteroidach, C++11
	for (auto& A : Asteroids)
	{
		//SprawdŸ statek
		const float ShipRadius = 15.0f;
		float r = A->GetRadius();
		sf::Vector2<float> p = A->GetPos();
		sf::Vector2<float> dist = p - Pos;
		float d = sqrt(dist.x*dist.x + dist.y*dist.y);

		if (d < r + ShipRadius)
		{
			//Statek wlecia³ w asteroidê
			A->SetShouldBeDestroyed(true);

			//Zmniejsz HP statku
			--HP;
			if (HP < 0) HP = 0;

			continue;
		}

		//SprawdŸ pociski
		const float BulletRadius = 3.0f;
		for (auto& B : Bullets)
		{
			sf::Vector2<float> pb = B->GetPos();
			dist = pb - p;
			d = sqrt(dist.x*dist.x + dist.y*dist.y);

			if (d < r + BulletRadius)
			{
				//Pocisk trafi³ asteroidê
				B->SetShouldBeDestroyed(true);

				//Usuñ HP z asteroidy za trafienie
				A->RemoveHP((int)B->GetPower());

				//Jeœli HP asteroidy równe jest zero, zniszcz j¹
				if (A->GetHP() == 0)
				{
					A->SetShouldBeDestroyed(true);

					//Dodaj punkty
					AddPoints(A->GetValue());

					//Wylosuj czy stworzyæ powerup na miejscu asteroidy
					if (rand() % 10 > 4)
					{
						Powerup* NewPwr = new Powerup(A->GetPos());
						Powerups.push_back(NewPwr);
					}

					//Stwórz mniejsze asteroidy na miejscu wiêkszej
					Asteroid* NewAst = nullptr;
					switch (A->GetType())
					{
					case 0:
						NewAst = new Asteroid("images/spaceship.png", 1, A->GetPos());
						Asteroids.push_back(NewAst);
						NewAst = new Asteroid("images/spaceship.png", 2, A->GetPos());
						Asteroids.push_back(NewAst);
						NewAst = new Asteroid("images/spaceship.png", 3, A->GetPos());
						Asteroids.push_back(NewAst);
						break;
					case 1:
						NewAst = new Asteroid("images/spaceship.png", 4, A->GetPos());
						Asteroids.push_back(NewAst);
						NewAst = new Asteroid("images/spaceship.png", 2, A->GetPos());
						Asteroids.push_back(NewAst);
						break;
					case 3:
						NewAst = new Asteroid("images/spaceship.png", 4, A->GetPos());
						Asteroids.push_back(NewAst);
						NewAst = new Asteroid("images/spaceship.png", 2, A->GetPos());
						Asteroids.push_back(NewAst);
						NewAst = new Asteroid("images/spaceship.png", 5, A->GetPos());
						Asteroids.push_back(NewAst);
						break;
					case 4:
						NewAst = new Asteroid("images/spaceship.png", 2, A->GetPos());
						Asteroids.push_back(NewAst);
						break;
					case 5:
						NewAst = new Asteroid("images/spaceship.png", 2, A->GetPos());
						Asteroids.push_back(NewAst);
						NewAst = new Asteroid("images/spaceship.png", 2, A->GetPos());
						Asteroids.push_back(NewAst);
						break;
					}
				}

				continue;
			}
		}
	}

	//Iteruj po wszystkich powerupach, C++11
	for (auto& P : Powerups)
	{
		const float ShipRadius = 15.0f;
		float r = P->GetRadius();
		sf::Vector2<float> p = P->GetPos();
		sf::Vector2<float> dist = p - Pos;
		float d = sqrt(dist.x*dist.x + dist.y*dist.y);

		if (d < r + ShipRadius)
		{
			//Zebranie powerupa
			P->SetShouldBeDestroyed(true);

			//Dodanie efektu
			switch (P->GetType())
			{
			case Powerup::TypeEnum::HealthUp:
				HP += 1;
				break;
			case Powerup::TypeEnum::BulletSpeedUp:
				BulletTimeInterval /= 1.1f;
				break;
			}
		}
	}

	return;
}
int Ship::GetHP()
{
	return HP;
}
int Ship::GetPoints()
{
	return Points;
}
void Ship::AddPoints(int how_many)
{
	Points += how_many;
	return;
}
void Ship::LoadConfigFile()
{
	//Spróbuj otworzyæ plik konfiguracyjny
	std::fstream File;
	File.open("ShipConfig.cfg", std::ios::in);

	if (!File.is_open())
	{
		std::cout << "Config file doesn't exist. Creating one..." << std::endl;
		File.open("ShipConfig.cfg", std::ios::out);

		//Parametry do zapisu
		const int HpToFile = 3;
		const int PointsToFile = 0;
		const int PosXToFile = 400;
		const int PosYToFile = 300;
		File << HpToFile << "\n" << PointsToFile << "\n" << PosXToFile << "\n" << PosYToFile << "\n";
	}
	else
	{
		//Odczytaj parametry z pliku
		File >> HP >> Points >> Pos.x >> Pos.y;
	}

	File.close();

	return;
}