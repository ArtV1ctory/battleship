#pragma once

#include "Defines.h"
#include <SFML/Graphics.hpp>

class Cell
{
public:
	Cell();
	void draw(sf::Texture& texture, sf::RenderWindow& window, int indent);
	bool isSelected = 0;
	bool isShip = 0;
	int number = 0;
	int length = 0;
	bool random = 0;
private:
	sf::Sprite  sprite;
	int getX();
	int getY();
};
