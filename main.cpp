#include<iostream>
#include"Game.h"



int main() {
	std::srand(static_cast<unsigned>(time(NULL)));
	Game game;
	
	std::cout << "hey";
	
	while (game.isOpen()) {
	
		if (game.gamestate == 0) {
			game.startScreen();
		}
		if (game.gamestate == 1) {
			game.update();
			game.render();
		}
		if (game.gamestate == 2) {
			game.endScreen();
		}
	}
}