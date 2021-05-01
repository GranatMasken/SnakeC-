#include "Game.h"

//Initialize all variables
void Game::initVariables()
{
	this->window = nullptr;
	this->scl = 20;
	this->gamestate = 0;
}
//Initialize the renderwindow.
void Game::initWindow()
{
	this->videoMode.height = 800;
	this->videoMode.width = 1600;
	this->window = new sf::RenderWindow(this->videoMode, "Gem", sf::Style::Titlebar | sf::Style::Close);
	this->window->setFramerateLimit(5);
}

void Game::intitPlayer()
{
	this->x = 800;
	this->y = 400;
	this->xSpeed = 1;
	this->ySpeed = 0;
	this->player.setPosition(this->x,this->y);
	this->player.setSize(sf::Vector2f(scl,scl));
	this->player.setFillColor(sf::Color::Green);

	this->total = 6;
}


//constructor
Game::Game()
{
	this->initVariables();
	this->initWindow();
	this->intitPlayer();

	//fruit
	this->spawn();
	this->initFruit();

	this->initAssets();

}
//deconstructor
Game::~Game()
{
	delete this->window;
}

void Game::pollEvents()
{
	while (this->window->pollEvent(this->ev)) {

		//Switch case för vilken typ av event
		switch (this->ev.type) {

		case sf::Event::Closed:
			this->window->close();
			break;
		case sf::Event::KeyPressed:
			if (this->ev.key.code == sf::Keyboard::Escape) {
				this->window->close();
				break;
			}
			//check if any movement ket is pressed and change the direction
			//up
			if (this->ev.key.code == sf::Keyboard::Up && this->checkBodyPos() != 1) {
				this->xSpeed = 0;
				this->ySpeed = -1;
				break;
			}
			//down
			if (this->ev.key.code == sf::Keyboard::Down && this->checkBodyPos() != 2) {
				this->xSpeed = 0;
				this->ySpeed = 1;;
				break;
			}
			//left
			if (this->ev.key.code == sf::Keyboard::Left && this->checkBodyPos() != 3) {
				this->xSpeed = -1;
				this->ySpeed = 0;
				break;
			}
			//right
			if (this->ev.key.code == sf::Keyboard::Right && this->checkBodyPos() != 4) {
				this->xSpeed = 1;
				this->ySpeed = 0;
				break;
			}

			//check if enter is pressed at start screen;
			if (this->ev.key.code == sf::Keyboard::Enter && this->gamestate == 0 ) {
				this->gamestate = 1;
				std::cout << "start menu closed \n";
				break;
			}

		}

	}
}

void Game::update()
{
	this->pollEvents();
	this->updatePlayer();
}

void Game::render()
{
	this->window->clear(sf::Color(50,50,50,255));

	//begin draw

	this->renderPlayer();

	//render fruit
	this->window->draw(this->fruit);

	//render text

	this->text2.setString("Points: " + std::to_string(total));
	this->text2.setPosition(0,0);
	this->window->draw(text2);

	//end draw
	this->window->display();
}

//function that updates and mover the player
void Game::updatePlayer()
{
	//moving snake and adding bodies
	if (this->total > 0) {
		if (this->total == this->tail.size() && !this->tail.empty()) {

			tail.erase(this->tail.begin());

		}
		tail.push_back(player);
	}
	std::cout << tail.size() << std::endl;

	//movement
	this->player.move(xSpeed*scl,ySpeed*scl);

	this->touchingBody();
	this->eat();
}
//function that draws the player onto the screen
void Game::renderPlayer()
{
	this->player.setFillColor(sf::Color(0,200,0,255));
	this->window->draw(this->player);
	this->player.setFillColor(sf::Color::Green);
	for (auto& e : this->tail) {
		this->window->draw(e);
	}
}

/*
	Checks where the body is relative to the head.
	returns int
	1=above
	2=below
	3=left
	4=right
*/
int Game::checkBodyPos()
{

	if (this->tail.back().getPosition().y < player.getPosition().y) {
		return 1;
	}
	if (this->tail.back().getPosition().y > player.getPosition().y) {
		return 2;
	}
	if (this->tail.back().getPosition().x < player.getPosition().x) {
		return 3;
	}
	if (this->tail.back().getPosition().x > player.getPosition().x) {
		return 4;
	}
	return 0;
}

//checks if snake is touching itself
void Game::touchingBody()
{
	for (auto& e : this->tail) {
		if (this->player.getGlobalBounds().contains(sf::Vector2f(e.getPosition()))) {
			std::cout << "dead \n";
			this->gamestate = 2;
		}
	}

}

//check if snake is touching food
void Game::eat()
{
	if (player.getGlobalBounds().contains(fruit.getPosition())) {
		total += 2;
		spawn();
	}
}

const bool Game::isOpen() const
{
	if (this->window->isOpen())
		return true;
	else
		return false;
}

// function that sets up all text, pictures, sounds and more;
void Game::initAssets()
{

	if (!this->font.loadFromFile("Assets/Fonts/KarmaFuture.ttf")) {
		std::cout << "Error loading font" << std::endl;
		system("pause");
	}

	this->text1.setFont(font);
	//this->text1.setColor(sf::Color::White);
	this->text1.setCharacterSize(24);

	this->text2.setFont(font);
	//this->text2.setColor(sf::Color::White);
	this->text2.setCharacterSize(24);

	//loads highscore form textfile
	std::ifstream textfile("Assets/Saves/highscore.txt");
	while (std::getline(textfile, highscoreString)) {
		highscore = std::stoi(highscoreString);
		std::cout << std::to_string(highscore);
	}
	textfile.close();

}

void Game::save()
{
	std::ofstream textSave("Assets/Saves/highscore.txt");
	textSave << std::to_string(total);
	textSave.close();
}


//fruit functions

void Game::initFruit()
{
	this->fruit.setSize(sf::Vector2f(this->scl -5, this->scl-5));
	this->fruit.setFillColor(sf::Color::Red);
	this->fruit.setOutlineColor(sf::Color(150, 0, 0,255));
	this->fruit.setOutlineThickness(2);

}


void Game::spawn()
{
	pickPos();
	while (touchingPlayer()) {
		pickPos();
	}

}

void Game::pickPos()
{
	int randX = rand() % this->window->getSize().x / this-> scl -1;
	int randY = rand() % this->window->getSize().y / this->scl -1;

	this->fruit.setPosition(sf::Vector2f(randX * scl +2.5, randY * scl + 2.5));
	
}

bool Game::touchingPlayer()
{
	bool x = false;

	for (auto& e : this->tail) {
		if (this->fruit.getGlobalBounds().contains(e.getPosition())) {
			x = true;
		}
	}
	return x;
}

void Game::startScreen()
{
	this->pollEvents();

	//render start screen
	this->window->clear(sf::Color(50, 50, 50, 255));

	//text
	this->text1.setString("Press Enter to Play");
	this->text1.setCharacterSize(50);
	this->text1.setPosition(this->window->getSize().x /2 - this->text1.getGlobalBounds().width / 2, 300);
	this->window->draw(text1);

	this->text2.setString("highscore: " + highscoreString);
	this->text2.setCharacterSize(24);
	this->text2.setPosition(this->window->getSize().x / 2 - this->text2.getGlobalBounds().width / 2, 400);
	this->window->draw(text2);

	//end draw
	this->window->display();

}

void Game::endScreen()
{
	pollEvents();
	if (total > highscore) {
		this->save();
		this->text1.setString("Score: " + std::to_string(total) + " New HighScore!");
		this->text1.setPosition(this->window->getSize().x / 2 - this->text1.getGlobalBounds().width / 2, 300);
		this->text2.setString("Old highscore: " + std::to_string(this->highscore));
		this->text2.setPosition(this->window->getSize().x / 2 - this->text2.getGlobalBounds().width / 2, 400);
	}
	else {
		this->text1.setString("Score: " + std::to_string(total));
		this->text1.setPosition(this->window->getSize().x / 2 - this->text1.getGlobalBounds().width / 2, 300);
		this->text2.setString("Highscore: " + std::to_string(this->highscore));
		this->text2.setPosition(this->window->getSize().x / 2 - this->text2.getGlobalBounds().width / 2, 400);
	}

	this->window->clear(sf::Color(50, 50, 50, 255));

	this->window->draw(text1);
	this->window->draw(text2);


	this->window->display();
}
