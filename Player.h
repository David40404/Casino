#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;
using namespace sf;

class Player {
	Text textBalance;
public:
	unsigned long long balance;
	long long betSum;

	Player(Font& font, unsigned long long balance = 0) : balance(balance), betSum(1) {
		textBalance.setFont(font);
		textBalance.setPosition({ 5, 5 });
	}

	bet_t makeBet(unsigned long long sum, unsigned short number = NOT_NUMBER, unsigned short field = NOT_FIELD) {
		bet_t bet;

		if (balance >= sum) {
			bet.sum = sum;
			balance -= sum;
		} else {
			cout << "You doesn't have " << sum << "$\n";
		}

		bet.field = field;
		bet.number = number;

		return bet;
	}

	void drawBalance(RenderWindow& window) {
		textBalance.setString(to_string(balance) + "$");
		window.draw(textBalance);
	}

	void loadBalance() {
		ifstream bFile("Saves/balance.txt", std::ios::binary);

		bFile.read((char*)(&balance), sizeof(balance));

		bFile.close();
	}

	void saveBalance() {
		ofstream bFile("Saves/balance.txt", std::ios::binary);

		bFile.write((char*)(&balance), sizeof(balance));

		bFile.close();
	}
};
