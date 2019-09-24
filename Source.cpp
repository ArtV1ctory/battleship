#include "Cell.h"
#include "Defines.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>

sf::RenderWindow window(sf::VideoMode(561, 321), "SEA BATTLE");
sf::Texture ship, tBackground, cross, shipRed, dot, tWaiting, end;
sf::Sprite sBackground, sWaiting;
sf::Font font;
sf::Text text("", font, 20);
bool click_on_target, player, gameOver = false;
bool click_for_continue = true;
int x, y = 0;

bool notBan(int x, int y, int parts, int random, std::vector<Cell>& myField) {
	if (!myField[x + y * LENGTH_ARRAY].isShip) {
		if (myField[x - 1 + y * LENGTH_ARRAY].isShip || myField[x - 1 + (y - 1) * LENGTH_ARRAY].isShip
			|| myField[x + (y - 1) * LENGTH_ARRAY].isShip || myField[x + 1 + (y - 1) * LENGTH_ARRAY].isShip
			|| myField[x + 1 + (y)* LENGTH_ARRAY].isShip || myField[x - 1 + (y + 1) * LENGTH_ARRAY].isShip
			|| myField[x + 1 + (y + 1) * LENGTH_ARRAY].isShip || myField[x + (y + 1) * LENGTH_ARRAY].isShip)
			return false;
	}
	if (random == 0) {
		for (int i = 0; i < parts + 1; i++) {
			if (myField[x + i + (y * LENGTH_ARRAY)].isShip || myField[x + i + (y + 1) * LENGTH_ARRAY].isShip
				|| myField[x + i + (y - 1) * LENGTH_ARRAY].isShip)
				return false;
		}
	}
	else if (random == 1) {
		for (int i = 0; i < parts + 1; i++) {
			if (myField[x + (y + i) * LENGTH_ARRAY].isShip || myField[x + 1 + (y + i) * LENGTH_ARRAY].isShip
				|| myField[x - 1 + (y + i) * LENGTH_ARRAY].isShip)
				return false;
		}
	}
	return true;
}

void alignment(std::vector<Cell>& myField) {
	int n = 0;
	for (auto it = myField.begin(); it < myField.end(); *it++) {
		it->number = n++;
	}
	n = 1;
	for (int parts = 4; parts >= 1; parts--) {
		for (int i = 1; i <= n; i++) {
			bool random = rand() % 2;
			int k = 0;
			int x = (rand() % 7) + 1;
			int y = (rand() % 10) + 1;
			if (random == 0) {
				if (notBan(x, y, parts, random, myField)) {
					while (k < parts) {
						myField[(x + k) + LENGTH_ARRAY * y].isShip = true;
						myField[(x + k) + LENGTH_ARRAY * y].random = random;
						myField[(x + k) + LENGTH_ARRAY * y].length = parts;
						k++;
					}
				}
				else i--;
			}
			else {
				if (notBan(y, x, parts, random, myField)) {
					while (k < parts) {
						myField[y + (x + k) * LENGTH_ARRAY].isShip = true;
						myField[y + (x + k) * LENGTH_ARRAY].random = random;
						myField[y + (x + k) * LENGTH_ARRAY].length = parts;
						k++;
					}
				}
				else i--;
			}
		}
		n++;
	}

	//Output of all fields in text form to standard output
	/*for (int j = 12; j < 131; j++) {
		if (j % 12 == 0) {
			for (int i = 0; i <= 9; i++) {
				++j;
				std::cout << myField.at(j).isShip << " ";
			}
			std::cout << std::endl;
		}
	}
	std::cout << std::endl;*/
}

void actions(std::vector<Cell>& myField, std::vector<Cell>& myTarget) {
	if (!click_for_continue) {
		window.display();
		window.clear();
		window.draw(sWaiting);
		return;
	}
	auto myIter = myField.begin(), target = myTarget.begin();
	int kills = 0;
	if (click_on_target) {
		click_on_target = false;
		int i = ((x - INDENT_RIGHT) / SIZE + (y - INDENT_UP) / SIZE * LENGTH_ARRAY);
		if (!target[i].isSelected) {
			target[i].isSelected = true;
			target += i;
			if (!target->isShip) {
				click_for_continue = false;
				player = !player;
			}
			else {
				if (target->random == 0) {
					while ((target - 1)->isShip && (target - 1)->isSelected) {
						target--;
					}
					for (int k = 1; k < target->length; k++) {
						if (!(target + k)->isShip && target->length > 1 || ((target + k)->isShip && !(target + k)->isSelected))
							return;
					}
					(target - 1)->isShip = false;
					(target - 1)->isSelected = true;
					(target + target->length)->isShip = false;
					(target + target->length)->isSelected = true;
					for (int k = 0; k <= target->length + 1; k++) {
						(target + LENGTH_ARRAY - 1 + k)->isSelected = true;
						(target + LENGTH_ARRAY - 1 + k)->isShip = false;
						(target - (LENGTH_ARRAY + 1) + k)->isSelected = true;
						(target - (LENGTH_ARRAY + 1) + k)->isShip = false;
					}
				}
				else {
					while ((target - LENGTH_ARRAY)->isShip && (target - LENGTH_ARRAY)->isSelected) {
						target -= LENGTH_ARRAY;
					}
					for (int k = 1; k < target->length; k++) {
						if (!(target + k * LENGTH_ARRAY)->isShip && target->length > 1
							|| ((target + k * LENGTH_ARRAY)->isShip && !(target + k * LENGTH_ARRAY)->isSelected))
							return;
					}
					(target - LENGTH_ARRAY)->isShip = false;
					(target - LENGTH_ARRAY)->isSelected = true;
					(target + target->length * LENGTH_ARRAY)->isShip = false;
					(target + target->length * LENGTH_ARRAY)->isSelected = true;
					for (int k = 0; k <= target->length + 1; k++) {
						(target - LENGTH_ARRAY - 1 + k * LENGTH_ARRAY)->isSelected = true;
						(target - LENGTH_ARRAY - 1 + k * LENGTH_ARRAY)->isShip = false;
						(target - LENGTH_ARRAY + 1 + k * LENGTH_ARRAY)->isSelected = true;
						(target - LENGTH_ARRAY + 1 + k * LENGTH_ARRAY)->isShip = false;
					}
				}
			}
		}
	}
	for (myIter = myField.begin(), target = myTarget.begin();
		myIter < myField.end(), target < myTarget.end();
		*myIter++, *target++) {
		if (myIter->isShip && !myIter->isSelected) {
			myIter->draw(ship, window, INDENT_LEFT);
		}
		else if (myIter->isSelected && myIter->isShip) {
			myIter->draw(shipRed, window, INDENT_LEFT);
		}
		else if (myIter->isSelected && !myIter->isShip) {
			myIter->draw(dot, window, INDENT_LEFT);
		}
		if (!target->isShip && target->isSelected) {
			target->draw(dot, window, INDENT_RIGHT);
		}
		else if (target->isShip && target->isSelected) {
			target->draw(shipRed, window, INDENT_RIGHT);
			kills++;
			if (kills == 20) {
				gameOver = true;
			}
		}
	}
	std::ostringstream playerString;
	playerString << player + 1;
	if (gameOver) {
		sBackground.setTexture(end);
		text.setCharacterSize(40);
		text.setPosition(40, 119);
		text.setString("Player " + playerString.str() + " is the winner!");
		window.clear();
		window.draw(sBackground);
		window.draw(text);
		window.display();
		return;
	}
	text.setString("Player " + playerString.str());
	window.display();
	window.clear();
	window.draw(sBackground);
	window.draw(text);
}

int main()
{
	srand(time(NULL));
	std::vector<Cell> field(144), field2(144);
	alignment(field);
	alignment(field2);

	end.loadFromFile("source/8_1111.png");
	font.loadFromFile("source/segoe_print.ttf");
	text.setFillColor(sf::Color::Red);
	text.setPosition(239, 0);
	ship.loadFromFile("source/ship.png");
	tBackground.loadFromFile("source/8_11.png");
	sBackground.setTexture(tBackground);
	sBackground.setPosition(0, 0);
	tWaiting.loadFromFile("source/8_111.png");
	sWaiting.setTexture(tWaiting);
	sWaiting.setPosition(0, 0);
	cross.loadFromFile("source/shot.png");
	shipRed.loadFromFile("source/shipRed.png");
	dot.loadFromFile("source/dot.png");

	while (window.isOpen())
	{
		sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
		sf::Vector2f pos = window.mapPixelToCoords(pixelPos);
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left
				&& pos.x >= INDENT_RIGHT && pos.x <= INDENT_RIGHT2
				&& pos.y >= INDENT_UP && pos.y <= INDENT_DOWN && click_for_continue) {
				click_on_target = true;
				x = pos.x + SIZE;
				y = pos.y + SIZE;
			}
			if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left
				&& !click_for_continue) {
				click_for_continue = true;
			}
		}
		if (!player) {
			actions(field, field2);
		}
		else {
			actions(field2, field);
		}
	}
	field.clear();
	field2.clear();
	return 0;
}
