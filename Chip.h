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
	Text ñhipText;

	Chip(const unsigned long long value = 0, const Color color = Color(192, 128, 128), const Vector2f position = { 0, 0 }) {
		chipShape.setFillColor(color);
		chipShape.setRadius(15);

		ñhipText.setString(to_string(value) + "$");
		ñhipText.setFillColor(Color::Black);

	}

	void setCharSize(unsigned charSize) {
		ñhipText.setCharacterSize(charSize);
	}

	void setFont(const sf::Font& font) {
		ñhipText.setFont(font);
	}

	void setPosition(const sf::Vector2f point) {
		chipShape.setPosition(point);

		float xPos = (point.x + chipShape.getRadius()) - (ñhipText.getLocalBounds().width / 2);
		float yPos = (point.y + chipShape.getRadius() / 1.2) - (ñhipText.getLocalBounds().height / 2);

		ñhipText.setPosition(xPos, yPos);
	}


	void draw(RenderWindow& window) const {
		window.draw(chipShape);
		window.draw(ñhipText);
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