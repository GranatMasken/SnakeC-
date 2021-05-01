#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#include <iostream>

#include <fstream>

#include <string>

#include <sstream>


class Game
{
private:

	//window
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event ev;

	//private functions
	void initVariables();
	void initWindow();
	void intitPlayer();


	

	//player
	int total;	
	float x;
	float y;
	float xSpeed;
	float ySpeed;

	//game logic
	int scl;


	//player object
	sf::RectangleShape player;
	std::vector<sf::RectangleShape> tail;

	//fruit init
	void initFruit();

	//fruit object
	sf::RectangleShape fruit;


	//inti assets
	void initAssets();

	sf::Text text1;
	sf::Text text2;
	sf::Font font;

	//save highscore
	void save();

	int highscore;
	std::string highscoreString;

public:


	//constructor / deconstructor
	Game();
	~Game();

	//public functions
	void pollEvents();
	void update();
	void render();



	//player functions
	void updatePlayer();
	void renderPlayer();


	int checkBodyPos();
	void touchingBody();
	void eat();


	//cummunication with other classes
	const bool isOpen() const;

	//fruit functions
	void spawn();
	void pickPos();
	bool touchingPlayer();


	//other gamestates
	void startScreen();
	void endScreen();

	//gamestate variable
	//gamestate variable
	int gamestate;
};

