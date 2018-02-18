#include <iostream>

#include "Gameplay.h"

int main()
{
	/*
	Dzia�anie:
	Najwa�niejsz� klas� jest Gameplay. Klasa ta kontroluje ca�� rozgrywk� i gromadzi dane niezb�dne w grze. Odpowiada za
	rysowanie obiekt�w na ekranie, ich spawnowanie i usuwanie. Znajduje si� w niej obiekt statku kosmicznego Ship oraz
	kontenery na asteroidy i pociski. Kontrola nad pociskami realizowana jest w klasie Gameplay, natomiast za pociski
	odpowiada klasa Ship.

	Metoda Run() z klasy Gameplay uruchamia okno SFML i wchodzi w p�tl� g��wn� programu. P�tla g��wna odbiera komunikaty SFML,
	rysuje obiekty na scenie oraz usuwa te, kt�re zosta�y oznaczone jako gotowe do usuni�cia.
	
	Hierarchia dziedziczenia jest nast�puj�ca: klas� podstawow� jest Object, b�d�ca og�ln� reprezentacj� obiektu do narysowania.
	Posiada metod� prawdziwie wirtualn� GetSprite(), kt�ra musi zosta� zaimplementowana dla ka�dej klasy pochodnej. Zadaniem tej
	metody jest ostatecznie zwr�cenie sprajta/obrazka, kt�ry ma zosta� narysowany przez klas� Gameplay. Jednak dziej� si� tam
	tak�e wszystkie inne rzeczy odpowiadaj�ce za aktualizacj� obiekt�w na scenie: poruszanie, animacj�, kolizj� i input od gracza.
	Po klasie Object dziedziczy Ship, b�d�cy reprezentacj� statku kosmicznego gracza. Statek kosmiczny jest poruszany przez
	gracza. Mo�e on strzela�, porusza� si� do przodu i na boki oraz skr�ca�. W zale�no�ci od stanu ruchu, w metodzie CheckInput
	wybierana jest odpowiednia klatka animacji. Statek przy ruchu do przodu generuje animowany p�omie� z dysz. Animacja
	odbywa si� poprzez cykliczne prze��czanie klatek animacji. Podczas strza�u generowany jest pocisk, kt�ry nast�pnie
	dodawany jest do listy wszystkich pocisk�w. W ten spos�b mo�liwe jest wyrenderowanie pocisk�w w klasie Gameplay.
	Strza� obs�ugiwany jest w metodzie void Ship::CheckInput().
	Wygenerowany pocisk musi zosta� obr�cony zgodnie z obrotem statku. Odbywa si� to za pomoc� klasy transformacji SFML: sf:Transform.
	Inn� klas� dziedzicz�c� po Object jest klasa Destroyable. Reprezentuje ona obiekty, kt�re mog� by� dynamicznie tworzone i niszczone.
	Takimi obiektami s� asteroidy Asteroid i pociski Bullet, kt�re dziedzicz� po klasie Destroyable. Asteroidy generowane s�
	w sta�ych odst�pach czasowych przez klas� Gameplay. Posiadaj� parametr zdrowia HP, kt�ry jest obni�any przez trafianie
	pociskami. Gdy HP spadnie do zera, asteroida jest niszczona w klasie Gameplay po ustawieniu parametru ShouldBeDestroyed zdefiniowanego
	w klasie Destroyable. Podobnie zachowuje si� statek kosmiczny Ship. Jego HP jest obni�ane przy uderzeniu w asteroid�. Po zmniejszeniu
	do zera, statek jest usuwany w klasie Gameplay i tworzony na nowo - z zerow� liczb� punkt�w i pe�nym HP.
	Klasa Explosion dziedziczy po klasie Object. Jest reprezentacj� animowanego wybuchu, kt�ry powstaje po znisczeniu asteroidy lub uderzeniu
	w statek. Eksplozja jest generowana przez klas� Gameplay podczas niszczenia obiektu asteroidy. Dzia�anie klasy explosion polega
	na wybraniu losowej animacji wybuchu, ustawieniu parametr�w animacji i ostatecznie samej animacji. Po zako�czeniu animacji,
	klasa Explosion ustawia parametr gotowo�ci do usuni�cia, po czym jest niszczona przez klas� Gameplay.

	Du�e asteroidy po zniszczeniu przez pocisk, rozpryskuj� si� na mniejsze. Ilo�� i typ mniejszych asteroid jest wybierany w zale�no�ci
	od typu du�ej asteroidy. Proces ten odbywa si� w metodzie void Ship::CheckCollision().

	Po zniszczeniu asteroidy jest szansa (wyliczana za pomoc� funkcji rand()) na wytworzenie powerupu na miejscu zniszczonej asteroidy.
	Powerup jest tworzony w metodzie Ship::CheckCollision() i le�y na planszy przez 10 sekund. Je�li nie zostanie zebrany, znika.
	Niszczeniem powerupu zajmuje si� klasa Gameplay.

	Do obs�ugi wyj�tk�w zwi�zanych z wylatywaniem statku kosmicznego poza ekran zosta� u�yty blok try/catch. Znajduje si� on
	w metodzie Shipp::CheckInput() i jest wywo�ywany, gdy statek znajdzie si� w ca�o�ci poza ekranem. Wtedy nast�puje wypisanie
	informacji o wyst�pieniu wyj�tku, a statek jest przemieszczany na drug� stron� ekranu.

	Istnieje mo�liwo�� ustawienia w�asnych warto�ci pocz�tkowych statku: pocz�tkowego HP, punkt�w i pozycji. Odbywa si� to w metodzie
	void Ship::LoadConfigFile(). Sprawdzane jest, czy istnieje plik konfiguracyjny ShipConfig.cfg. Je�li nie istnieje, tworzony jest
	nowy z domy�lnymi warto�ciami. Je�li istnieje, odczytywane s� z niego parametry. Gracz, poprzez edycj� pliku, mo�e sam ustali�
	z jakiej pozycji startuje statek, ile powinien otrzyma� punkt�w zdrowia i ile punkt�w powinien mie� na pocz�tku.

	Sterowanie odbywa si� za pomoc� klawiszy strza�ek, A, D do poruszania i LCTRL do strzelania. Klawiszem ESC wychodzimy z gry
	Zadaniem gracza jest uzbieranie jak najwi�kszej liczby punkt�w za zestrzeliwanie asteroid przy ograniczonej liczbie punkt�w �ycia.
	*/

	//Elementy obiektowo�ci:
	//- Dziedziczenie: Object -> Ship, Explosion; Object -> Destroyable -> Asteroid, Bullet, Powerup
	//- Hermetyzacja: private, protected i public w klasach, metody dost�powe
	//- Abstrakcja: ka�da klasa jest elementem, kt�ry co� sob� reprezentuje i ma jakie� zadanie
	//- Polimorfizm: metoda GetSprite() w klasie Object, implementowana r�nie dla klas pochodnych

	//Problemy:
	//- wyb�r kontenera na pociski (vector, list itd.)
	//- przypadkowe przes�anie listy pocisk�w przez warto�� zamiast przez referencj� sko�czy�o si�
	//  wyst�pieniem wyj�tku
	//- przypadkowe usuni�cie obiektu z listy podczas iteracji po jej elementach powodowa�o wyst�pienie wyj�tku
	//- W programie wyst�puje wielokrotne wczytywanie jednej tekstury. Powinno si� j� wczyta� raz i trzyma� w jednym miejscu,
	//  gdy� inaczej mo�e to prowadzi� do przepe�nienia pami�ci i/lub spadku wydajno�ci. Ka�dy obiekt wykorzystuj�cy tekstur�
	//  powinien otrzyma� tylko jej referencj� lub wska�nik na ni�.
	//- Sprawdzanie kolizji jest zasobo�erne. Nast�puje sprawdzanie 'ka�dy z ka�dym' i przy du�ej ilo�ci asteroid i pocisk�w
	//  na mapie mo�e wyst�pi� spadek wydajno�ci. Mo�na dokona� optymalizacji poprzez zastosowanie drzewa czw�rkowego.
	//- Tekstury z animacj� eksplozji s� du�e i ich wczytywanie w czasie rzeczywistym jest niemo�liwe bez skok�w animacji.
	//  Trzeba je wczyta� tylko raz i przekazywa� wska�nik do klasy eksplozji.
	

	class Gameplay Game; //g��wna klasa odpowiedzialna za rozgrywk�

	Game.Run();

	return 0;
}