#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace sf;

#include "Button.h"
#include "Player.h"

class AdminPanel {
	RectangleShape adminPanel;
	Player* player;
	bool isActivateFlag;
	bool isDelayOn;
public:
	Button addMoneyToBalanceButton;
	Button delayTurnButton;
	Button closeAdminPanelButton;

	AdminPanel(Player* player, Font* font, bool isActivateFlag = false) :
		isActivateFlag(isActivateFlag), player(player),
		addMoneyToBalanceButton("Money", { 100, 40 }, 25, Color(70, 80, 80), Color::Black),
		delayTurnButton("RDelay", {100, 40}, 20, Color(70, 80, 80), Color::Black),
		closeAdminPanelButton("X", { 30, 30 }, 18, Color(160, 70, 70), Color::White),
		isDelayOn(false) {

		adminPanel.setSize({ 110, 200 });
		adminPanel.setPosition({600, 5});
		adminPanel.setFillColor(Color(100, 120, 130));
	}

	void draw(RenderWindow& window) {
		if (isActivateFlag) {
			window.draw(adminPanel);
			addMoneyToBalanceButton.draw(window);
			closeAdminPanelButton.draw(window);
			delayTurnButton.draw(window);
		}
	}

	void addMoney() {
		player->balance += player->betSum;
	}

	void delayTurn() {
		isDelayOn = !isDelayOn;
	}

	bool& delayIsOn() {
		return isDelayOn;
	}

	bool& isActivate() {
		return isActivateFlag;
	}
};