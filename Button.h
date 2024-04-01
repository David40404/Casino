#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace sf;

class Button {
	RectangleShape button;
	Text text;
	Vector2f buttonSize;
public:
	Button(Font& font, const string btnText = "Button", const Vector2f buttonSize = { 100, 60 }, const int charSize = 25, const Color bgColor = Color(128, 128, 128), const Color textColor = Color::Black) :
		buttonSize(buttonSize) {

		button.setSize(buttonSize);
		button.setFillColor(bgColor);

		text.setString(btnText);
		text.setCharacterSize(charSize);
		text.setFillColor(textColor);

		text.setFont(font);
	}

	Button(const string btnText = "Button", const Vector2f buttonSize = { 100, 60 }, const int charSize = 25, const Color bgColor = Color(128, 128, 128), const Color textColor = Color::Black) :
		buttonSize(buttonSize) {

		button.setSize(buttonSize);
		button.setFillColor(bgColor);

		text.setString(btnText);
		text.setCharacterSize(charSize);
		text.setFillColor(textColor);
	}

	void setFont(const Font& fonts) {
		text.setFont(fonts);
	}

	void setBackColor(const Color color) {
		button.setFillColor(color);
	}

	void setTextColor(const Color color) {
		text.setFillColor(color);
	}

	void setPosition(const Vector2f point) {
		button.setPosition(point);

		const float xTextPos = (point.x + buttonSize.x / 2) - (text.getLocalBounds().width / 2);
		const float yTextPos = (point.y + buttonSize.y / 2.2) - (text.getLocalBounds().height / 1.8);

		text.setPosition(xTextPos, yTextPos);
	}

	void setString(const string s_text) {
		text.setString(s_text);
	}

	void draw(RenderWindow& window) const {
		window.draw(button);
		window.draw(text);
	}

	bool isMouseOver(const RenderWindow& window) const {
		const Vector2i mousePos = Mouse::getPosition(window);

		const Vector2f btnPos = button.getPosition();

		const float btnWidth = btnPos.x + buttonSize.x;
		const float btnHeight = btnPos.y + buttonSize.y;

		if (mousePos.x < btnWidth && mousePos.x > btnPos.x && mousePos.y < btnHeight && mousePos.y > btnPos.y)
			return true;
		return false;
	}

	string getString() const {
		return text.getString();
	}
};