#include <iostream>

#include "Gameplay.h"

int main()
{
	/*
	Dzia³anie:
	Najwa¿niejsz¹ klas¹ jest Gameplay. Klasa ta kontroluje ca³¹ rozgrywkê i gromadzi dane niezbêdne w grze. Odpowiada za
	rysowanie obiektów na ekranie, ich spawnowanie i usuwanie. Znajduje siê w niej obiekt statku kosmicznego Ship oraz
	kontenery na asteroidy i pociski. Kontrola nad pociskami realizowana jest w klasie Gameplay, natomiast za pociski
	odpowiada klasa Ship.

	Metoda Run() z klasy Gameplay uruchamia okno SFML i wchodzi w pêtlê g³ówn¹ programu. Pêtla g³ówna odbiera komunikaty SFML,
	rysuje obiekty na scenie oraz usuwa te, które zosta³y oznaczone jako gotowe do usuniêcia.
	
	Hierarchia dziedziczenia jest nastêpuj¹ca: klas¹ podstawow¹ jest Object, bêd¹ca ogóln¹ reprezentacj¹ obiektu do narysowania.
	Posiada metodê prawdziwie wirtualn¹ GetSprite(), która musi zostaæ zaimplementowana dla ka¿dej klasy pochodnej. Zadaniem tej
	metody jest ostatecznie zwrócenie sprajta/obrazka, który ma zostaæ narysowany przez klasê Gameplay. Jednak dziej¹ siê tam
	tak¿e wszystkie inne rzeczy odpowiadaj¹ce za aktualizacjê obiektów na scenie: poruszanie, animacjê, kolizjê i input od gracza.
	Po klasie Object dziedziczy Ship, bêd¹cy reprezentacj¹ statku kosmicznego gracza. Statek kosmiczny jest poruszany przez
	gracza. Mo¿e on strzelaæ, poruszaæ siê do przodu i na boki oraz skrêcaæ. W zale¿noœci od stanu ruchu, w metodzie CheckInput
	wybierana jest odpowiednia klatka animacji. Statek przy ruchu do przodu generuje animowany p³omieñ z dysz. Animacja
	odbywa siê poprzez cykliczne prze³¹czanie klatek animacji. Podczas strza³u generowany jest pocisk, który nastêpnie
	dodawany jest do listy wszystkich pocisków. W ten sposób mo¿liwe jest wyrenderowanie pocisków w klasie Gameplay.
	Strza³ obs³ugiwany jest w metodzie void Ship::CheckInput().
	Wygenerowany pocisk musi zostaæ obrócony zgodnie z obrotem statku. Odbywa siê to za pomoc¹ klasy transformacji SFML: sf:Transform.
	Inn¹ klas¹ dziedzicz¹c¹ po Object jest klasa Destroyable. Reprezentuje ona obiekty, które mog¹ byæ dynamicznie tworzone i niszczone.
	Takimi obiektami s¹ asteroidy Asteroid i pociski Bullet, które dziedzicz¹ po klasie Destroyable. Asteroidy generowane s¹
	w sta³ych odstêpach czasowych przez klasê Gameplay. Posiadaj¹ parametr zdrowia HP, który jest obni¿any przez trafianie
	pociskami. Gdy HP spadnie do zera, asteroida jest niszczona w klasie Gameplay po ustawieniu parametru ShouldBeDestroyed zdefiniowanego
	w klasie Destroyable. Podobnie zachowuje siê statek kosmiczny Ship. Jego HP jest obni¿ane przy uderzeniu w asteroidê. Po zmniejszeniu
	do zera, statek jest usuwany w klasie Gameplay i tworzony na nowo - z zerow¹ liczb¹ punktów i pe³nym HP.
	Klasa Explosion dziedziczy po klasie Object. Jest reprezentacj¹ animowanego wybuchu, który powstaje po znisczeniu asteroidy lub uderzeniu
	w statek. Eksplozja jest generowana przez klasê Gameplay podczas niszczenia obiektu asteroidy. Dzia³anie klasy explosion polega
	na wybraniu losowej animacji wybuchu, ustawieniu parametrów animacji i ostatecznie samej animacji. Po zakoñczeniu animacji,
	klasa Explosion ustawia parametr gotowoœci do usuniêcia, po czym jest niszczona przez klasê Gameplay.

	Du¿e asteroidy po zniszczeniu przez pocisk, rozpryskuj¹ siê na mniejsze. Iloœæ i typ mniejszych asteroid jest wybierany w zale¿noœci
	od typu du¿ej asteroidy. Proces ten odbywa siê w metodzie void Ship::CheckCollision().

	Po zniszczeniu asteroidy jest szansa (wyliczana za pomoc¹ funkcji rand()) na wytworzenie powerupu na miejscu zniszczonej asteroidy.
	Powerup jest tworzony w metodzie Ship::CheckCollision() i le¿y na planszy przez 10 sekund. Jeœli nie zostanie zebrany, znika.
	Niszczeniem powerupu zajmuje siê klasa Gameplay.

	Do obs³ugi wyj¹tków zwi¹zanych z wylatywaniem statku kosmicznego poza ekran zosta³ u¿yty blok try/catch. Znajduje siê on
	w metodzie Shipp::CheckInput() i jest wywo³ywany, gdy statek znajdzie siê w ca³oœci poza ekranem. Wtedy nastêpuje wypisanie
	informacji o wyst¹pieniu wyj¹tku, a statek jest przemieszczany na drug¹ stronê ekranu.

	Istnieje mo¿liwoœæ ustawienia w³asnych wartoœci pocz¹tkowych statku: pocz¹tkowego HP, punktów i pozycji. Odbywa siê to w metodzie
	void Ship::LoadConfigFile(). Sprawdzane jest, czy istnieje plik konfiguracyjny ShipConfig.cfg. Jeœli nie istnieje, tworzony jest
	nowy z domyœlnymi wartoœciami. Jeœli istnieje, odczytywane s¹ z niego parametry. Gracz, poprzez edycjê pliku, mo¿e sam ustaliæ
	z jakiej pozycji startuje statek, ile powinien otrzymaæ punktów zdrowia i ile punktów powinien mieæ na pocz¹tku.

	Sterowanie odbywa siê za pomoc¹ klawiszy strza³ek, A, D do poruszania i LCTRL do strzelania. Klawiszem ESC wychodzimy z gry
	Zadaniem gracza jest uzbieranie jak najwiêkszej liczby punktów za zestrzeliwanie asteroid przy ograniczonej liczbie punktów ¿ycia.
	*/

	//Elementy obiektowoœci:
	//- Dziedziczenie: Object -> Ship, Explosion; Object -> Destroyable -> Asteroid, Bullet, Powerup
	//- Hermetyzacja: private, protected i public w klasach, metody dostêpowe
	//- Abstrakcja: ka¿da klasa jest elementem, który coœ sob¹ reprezentuje i ma jakieœ zadanie
	//- Polimorfizm: metoda GetSprite() w klasie Object, implementowana ró¿nie dla klas pochodnych

	//Problemy:
	//- wybór kontenera na pociski (vector, list itd.)
	//- przypadkowe przes³anie listy pocisków przez wartoœæ zamiast przez referencjê skoñczy³o siê
	//  wyst¹pieniem wyj¹tku
	//- przypadkowe usuniêcie obiektu z listy podczas iteracji po jej elementach powodowa³o wyst¹pienie wyj¹tku
	//- W programie wystêpuje wielokrotne wczytywanie jednej tekstury. Powinno siê j¹ wczytaæ raz i trzymaæ w jednym miejscu,
	//  gdy¿ inaczej mo¿e to prowadziæ do przepe³nienia pamiêci i/lub spadku wydajnoœci. Ka¿dy obiekt wykorzystuj¹cy teksturê
	//  powinien otrzymaæ tylko jej referencjê lub wskaŸnik na ni¹.
	//- Sprawdzanie kolizji jest zasobo¿erne. Nastêpuje sprawdzanie 'ka¿dy z ka¿dym' i przy du¿ej iloœci asteroid i pocisków
	//  na mapie mo¿e wyst¹piæ spadek wydajnoœci. Mo¿na dokonaæ optymalizacji poprzez zastosowanie drzewa czwórkowego.
	//- Tekstury z animacj¹ eksplozji s¹ du¿e i ich wczytywanie w czasie rzeczywistym jest niemo¿liwe bez skoków animacji.
	//  Trzeba je wczytaæ tylko raz i przekazywaæ wskaŸnik do klasy eksplozji.
	

	class Gameplay Game; //g³ówna klasa odpowiedzialna za rozgrywkê

	Game.Run();

	return 0;
}