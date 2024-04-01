#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace sf;

unsigned chipCharSize = 22;

class Chip {
public:
	CircleShape chipShape;
	Text �hipText;

	Chip(const unsigned long long value = 0, const Color color = Color(192, 128, 128), const Vector2f position = { 0, 0 }) {
		chipShape.setFillColor(color);
		chipShape.setRadius(15);

		�hipText.setString(to_string(value) + "$");
		�hipText.setFillColor(Color::Black);

	}

	void setCharSize(unsigned charSize) {
		�hipText.setCharacterSize(charSize);
	}

	void setFont(const sf::Font& font) {
		�hipText.setFont(font);
	}

	void setPosition(const sf::Vector2f point) {
		chipShape.setPosition(point);

		float xPos = (point.x + chipShape.getRadius()) - (�hipText.getLocalBounds().width / 2);
		float yPos = (point.y + chipShape.getRadius() / 1.2) - (�hipText.getLocalBounds().height / 2);

		�hipText.setPosition(xPos, yPos);
	}


	void draw(RenderWindow& window) const {
		window.draw(chipShape);
		window.draw(�hipText);
	}
};

void push_chip(Player& player, Array <Chip>& chips, RenderWindow& window, Font& font) {
	if (player.balance >= player.betSum) {
		chips.push_back(Chip(player.betSum, Color(100 + (player.betSum * 6463) % 155, 100 + (player.betSum * 3265) % 155, 100 + (player.betSum * 5426) % 155)));
		chips.back().setFont(font);

		Vector2f mousePosition = (Vector2f)Mouse::getPosition(window);
		mousePosition -= {15, 15};

		chips.back().setCharSize(chipCharSize);
		chips.back().setPosition(mousePosition);
	}
}