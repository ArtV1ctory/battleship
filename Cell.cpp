#include "Cell.h"

Cell::Cell()
{}

int Cell::getX() {
	return ((number % LENGTH_ARRAY) - 1) * SIZE;
}

int Cell::getY() {
	return ((number / LENGTH_ARRAY) - 1) * SIZE + INDENT_UP;
}

void Cell::draw(sf::Texture& texture, sf::RenderWindow& window, int indent) {
	if (((getX() + indent >= INDENT_LEFT && getX() + indent < INDENT_LEFT2)
		|| (getX() + indent >= INDENT_RIGHT && getX() + indent < INDENT_RIGHT2))
		&& getY() >= INDENT_UP && getY() < INDENT_DOWN) {
		sprite.setTexture(texture);
		sprite.setPosition(getX() + indent, getY());
		window.draw(sprite);
	}
}
