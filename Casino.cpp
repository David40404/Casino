#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <iostream>
#include <bitset>

using namespace std;
using namespace sf;

#include "Array.h"
#include "Roulette.h"
#include "Button.h"
#include "Player.h"
#include "Chip.h"
#include "AdminPanel.h"

static Font font;
static Roulette roulette;
static Player player(font, 100);
static AdminPanel adminPanel(&player, &font, false);

inline void ButtonSector(const unsigned short sectorNumber) {
	bet_t bet = player.makeBet(player.betSum, NOT_NUMBER, get_sector(sectorNumber));
	roulette.push_bet(bet);
}

inline void BetButton(const unsigned short number = NOT_NUMBER, const unsigned short field = NOT_FIELD) {
	bet_t bet = player.makeBet(player.betSum, number, field);
	roulette.push_bet(bet);
}

static Clock spin_clock;

void checkSpin(bool& isRouletteRotated, Button& fellOutNumber, Array <Chip>& chips, AdminPanel& adminPanel) {

	//cout << spin_clock.getElapsedTime().asSeconds() << '\n';

	float delay = ((adminPanel.delayIsOn()) ? 0.f : 4.f);

	if (isRouletteRotated and spin_clock.getElapsedTime().asSeconds() >= delay) {

		unsigned char roulette_number = roulette.choice_random();
		Array <bet_t> bets = roulette.analyze_bets(roulette_number);

		for (unsigned long long i = 0; i < bets.size(); ++i)
			cout << bets[i] << '\n';
		
		cout << '\n';
		cout << "Number: " << (int)roulette_number << "\n";

		for (unsigned long long i = 0; i < bets.size(); ++i)
			player.balance += bets[i].winning;

		roulette.update();

		fellOutNumber.setString(to_string(roulette_number));
		fellOutNumber.setPosition({ 450, 110 });


		if (roulette_number == 0)
			fellOutNumber.setBackColor(Color(32, 128, 32));
		else if ((roulette.identify_number(roulette_number) & 0b00000001) == black)
			fellOutNumber.setBackColor(Color(64, 64, 64));
		else
			fellOutNumber.setBackColor(Color(128, 32, 32));

		isRouletteRotated = false;
		chips.clear();
	}
}

inline void ButtonSpin(bool& isRouletteRotated) {
	//cout << "Spin!\n";

	if (!isRouletteRotated) {
		isRouletteRotated = true;
		spin_clock.restart();
	}
}

int main() {
	// Window
	RenderWindow window(VideoMode(800, 600), "Casino", Style::Titlebar, ContextSettings(0, 0, 8));
	window.setFramerateLimit(60);

	// Load saves
	player.loadBalance();

	// Event
	Event event;

	// Flags and Other
	bool isRouletteRotated = false;
	
	// Chips
	Array <Chip> chips;

	// Casino GUI
	font.loadFromFile("fonts/font.ttf");

	Button fellOutNumber(font, "?", { 100, 100 }, 30);
	fellOutNumber.setPosition({ 450, 110 });

	Texture roulette_sprite_texture;
	roulette_sprite_texture.loadFromFile("Textures/roulette.png");

	CircleShape roulette_sprite;
	roulette_sprite.setPosition({180, 150});
	roulette_sprite.setRadius(125);
	roulette_sprite.setTexture(&roulette_sprite_texture);
	roulette_sprite.setOrigin({ 125, 125 });

	Text betSumText;
	betSumText.setFont(font);
	betSumText.setCharacterSize(25);
	betSumText.setPosition({5, 505});

	// Color Buttons
	Button button_black(font, "Black", { 120, 40 }, 25, Color(40, 40, 40), Color::Black);
	button_black.setPosition({ 260, 500 });

	Button button_red(font, "Red", { 120, 40 }, 25, Color(200, 40, 40), Color::Black);
	button_red.setPosition({ 385, 500 });

	// Parity Buttons
	Button button_even(font, "Even", { 120, 40 }, 25, Color(90, 90, 90), Color::Black);
	button_even.setPosition({ 135, 500 });

	Button button_odd(font, "Odd", { 120, 40 }, 25, Color(90, 90, 90), Color::Black);
	button_odd.setPosition({ 510, 500 });

	// Sector Buttons
	Array <Button> sectorButtons;

	for (unsigned short i = 0; i < 3; ++i) {
		Button button_sector(font, to_string(i + 1) + "sector", {215, 45}, 25, Color(70, 70, 70), Color::Black);
		button_sector.setPosition({ 55.f + i * 220, 450 });

		sectorButtons.push_back(button_sector);
	}

	// Zero Button
	Button button_zero(font, "0", {50, 160}, 18, Color(30, 160, 30), Color::Black);
	button_zero.setPosition({0, 280});

	// Number Buttons
	Array <Button> numberButtons;

	for (unsigned short i = 1; i < 37; ++i) {

		Color numberColor = Color(200, 40, 40);

		bool isBlack = 0;
		for (int j = 0; j < 37; j++) {
			if (i == roulette.black_numbers[j]) {
				isBlack = 1;
				break;
			}
		}
			
		if (isBlack) numberColor = Color(70, 70, 70);

		Button button_number(font, to_string(i), { 50, 50 }, 18, numberColor, Color::Black);
		button_number.setPosition({ 55 + floor((float)(i - 1) / 3.f) * 55, (float)(390 - ((i - 1) % 3) * 55) });

		numberButtons.push_back(button_number);
	}

	// Spin Button
	Button button_spin(font, "Spin", { 75, 40 }, 25, Color(130, 100, 140), Color::Black);
	button_spin.setPosition({ 635, 500 });

	// Reset Button
	Button button_reset(font, "Reset", { 75, 30 }, 20, Color(180, 90, 110), Color::White);
	button_reset.setPosition({ 720, 5 });

	// Admin panel
	adminPanel.addMoneyToBalanceButton.setFont(font);
	adminPanel.addMoneyToBalanceButton.setPosition({ 605, 10 });

	adminPanel.closeAdminPanelButton.setFont(font);
	adminPanel.closeAdminPanelButton.setPosition({ 675, 170 });

	adminPanel.delayTurnButton.setFont(font);
	adminPanel.delayTurnButton.setPosition({ 605, 55 });

	Thread otherProcessesThread([&fellOutNumber, &chips, &isRouletteRotated]() {
		static const unordered_map <unsigned short, unsigned short> chipSizeMap = {
			{1000000, 6},
			{100000, 8},
			{10000, 9},
			{1000, 11},
			{100, 14},
			{10, 20},
			{1, 22 }
		};

		for (const auto chipSzElem : chipSizeMap)
			if (player.betSum < chipSzElem.first)
				chipCharSize = chipSzElem.second;

		checkSpin(isRouletteRotated, fellOutNumber, chips, adminPanel);

		});

	while (window.isOpen()) {

		while (window.pollEvent(event)) {
			if (Keyboard::isKeyPressed(Keyboard::Escape)) window.close();

			if (event.type == Event::Closed) window.close();

			if (event.type == Event::MouseWheelScrolled) {
				player.betSum += (1 * (event.mouseWheelScroll.delta));
				player.betSum = clamp(player.betSum, 1ll, 10000000ll);
			}

			for (size_t i = 0; i < 8; ++i) {
				if (Keyboard::isKeyPressed((Keyboard::Key)(Keyboard::Num1 + i))) {
					player.betSum = (long long)pow(10, i);
					break;
				}
			}

			if (event.type == Event::MouseButtonPressed) {
				if (!isRouletteRotated) {
					if (button_reset.isMouseOver(window)) { player.balance = 100; }

					if (button_black.isMouseOver(window)) { push_chip(player, chips, window, font); BetButton(NOT_NUMBER, black); }
					else if (button_red.isMouseOver(window)) { push_chip(player, chips, window, font); BetButton(NOT_NUMBER, red); }

					else if (button_even.isMouseOver(window)) { push_chip(player, chips, window, font); BetButton(NOT_NUMBER, even); }
					else if (button_odd.isMouseOver(window)) { push_chip(player, chips, window, font); BetButton(NOT_NUMBER, odd); }

					else if (button_zero.isMouseOver(window)) { push_chip(player, chips, window, font); BetButton(0, NOT_FIELD); }

					for (unsigned short i = 0; i < 3; ++i)
						if (sectorButtons[i].isMouseOver(window)) { push_chip(player, chips, window, font); ButtonSector(i + 1); }

					for (unsigned short i = 0; i < 36; ++i)
						if (numberButtons[i].isMouseOver(window)) { push_chip(player, chips, window, font); BetButton(i + 1, NOT_FIELD); }
				}

				if (adminPanel.isActivate()){
					if (adminPanel.addMoneyToBalanceButton.isMouseOver(window)) adminPanel.addMoney();
					if (adminPanel.closeAdminPanelButton.isMouseOver(window)) adminPanel.isActivate() = false;
					if (adminPanel.delayTurnButton.isMouseOver(window)) adminPanel.delayTurn();
				}

				if (button_spin.isMouseOver(window)) ButtonSpin(isRouletteRotated);
			}

			if (Keyboard::isKeyPressed((Keyboard::RShift)) and Keyboard::isKeyPressed((Keyboard::BackSpace))) adminPanel.isActivate() = true;
			if (Keyboard::isKeyPressed((Keyboard::Space))) ButtonSpin(isRouletteRotated);
		}

		otherProcessesThread.launch();

		window.clear(Color(100, 150, 120));

		button_black.draw(window);
		button_red.draw(window);

		button_even.draw(window);
		button_odd.draw(window);

		button_spin.draw(window);
		button_zero.draw(window);


		if (isRouletteRotated) {
			static float i = 0;
			roulette_sprite.rotate(abs(sin(i)) * 6);
			i += 0.0045;
		}

		window.draw(roulette_sprite);

		fellOutNumber.draw(window);

		betSumText.setString("Bet: " + to_string(player.betSum));
		window.draw(betSumText);

		player.drawBalance(window);

		for (unsigned short i = 0; i < 3; ++i)
			sectorButtons[i].draw(window);

		for (unsigned short i = 0; i < 37; ++i)
			numberButtons[i].draw(window);

		for (unsigned short i = 0; i < roulette.history.size(); ++i) {
			Color numberColor = Color(200, 40, 40);

			bool isBlack = 0;
			for (int j = 0; j < 37; j++) {
				if (roulette.history[i] == roulette.black_numbers[j]) {
					isBlack = 1;
					break;
				} else if (roulette.history[i] == 0) {
					numberColor = Color(30, 160, 30);
					break;
				}
			}

			if (isBlack) numberColor = Color(70, 70, 70);

			static Button button_number(font, "", {50, 50}, 18, numberColor, Color::Black);
			button_number.setString(to_string(roulette.history[i]));
			button_number.setBackColor(numberColor);
			button_number.setPosition({ 740, 50 + i * 52.f });

			button_number.draw(window);
		}

		for (unsigned short i = 0; i < chips.size(); ++i)
			chips[i].draw(window);

		button_reset.draw(window);

		adminPanel.draw(window);

		player.saveBalance();

		window.display();
	}
}