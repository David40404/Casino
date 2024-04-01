#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace sf;

#include "Array.h"
#include "DRand16.h"

#define NOT_NUMBER 37
#define NOT_FIELD  205

#define black   0b00000001
#define red     0b00000010

#define _1st    0b00000100
#define _2nd    0b00001000
#define _3rd    0b00001100

#define odd     0b00010000
#define even    0b00100000

#define _1_18   0b01000000
#define _19_36  0b10000000

typedef struct Bet {
	unsigned long long winning;
	unsigned long long sum;
	unsigned short number : 8;
	unsigned short field  : 8;

	Bet() : sum(0), number(0), field(0), winning(0) {
	}

	friend ostream& operator<<(ostream& out, const Bet& bet) {
		out << "Sum: " << bet.sum << " | Number: " << bet.number << " | Field: " << bet.field << " | Winning: " << bet.winning;
		return out;
	}
} bet_t;

unsigned short get_sector(const unsigned short sectorNumber) {
	switch (sectorNumber)
	{
	case 1: return _1st;
	case 2: return _2nd;
	case 3: return _3rd;
	default: return 4;
	}
}

class Roulette {
	Array <unsigned char> numbers;
	Array <bet_t> bets;
public:
	Array <unsigned char> history;

	const int black_numbers[18] = { 2, 4, 6, 8, 10, 11, 13, 15, 17, 20, 22, 24, 26, 28, 29, 31, 33, 35 };

	Roulette() : bets(0) {
		srand((unsigned)time(NULL));

		for (unsigned short i = 0; i < 37; ++i)
			numbers.push_back(i);
	}

	void update() {
		bets.clear();
	}

	Array <bet_t>& analyze_bets(const unsigned char number) {

		unsigned number_identify = identify_number(number);

		for (unsigned short i = 0; i < bets.size(); ++i) {
			bet_t* bet = &bets[i];

			bitset <8> bf = bet->field;
			bitset <8> bni = number_identify;
			cout << "Field: " << bf << " Identify: " << bni << " Number: " << (int)number << " Bet Number: " << bet->number << '\n';

			// Check Color
			if ((number_identify & 0b00000011) == bet->field) {
				bet->winning = bet->sum * 2;
			}
		
			// Check Sectors
			else if ((number_identify & 0b00001100) == bet->field) {
				bet->winning = bet->sum * 3;
			}
		
			// Check Parity
			else if ((number_identify & 0b00110000) == bet->field) {
				bet->winning = bet->sum * 2;
			}
		
			// Check Halfs
			else if ((number_identify & 0b11000000) == bet->field) {
				bet->winning = bet->sum * 2;
			}
		
			// Check Number
			else if (bet->number == number) {
				bet->winning = bet->sum * 36;
			}
		}

		return bets;
	}

	unsigned char choice_random() {
		unsigned char number = numbers[DRand16() % numbers.size()];
		history.push_begin(number);

		if (history.size() > 10) history.pop_back();

		return number;
	}

	unsigned short identify_number(const unsigned short number) const {
		unsigned short identify = 0;

		bool isBlack = 0;
		for (int i = 0; i < 18; i++) {
			if (number == black_numbers[i]) {
				isBlack = 1;
				break;
			}
		}

		if (isBlack) identify |= black;
		else identify |= red;

		if (number >= 1 and number <= 12) identify |= _1st;
		else if (number >= 13 and number <= 24) identify |= _2nd;
		else if (number >= 25 and number <= 36) identify |= _3rd;

		if (number % 2 == 0) identify |= even;
		else identify |= odd;

		if (number > 0 and number < 19) identify |= _1_18;
		else if (number > 18 and number < 37) identify |= _19_36;

		return identify;
	}

	void push_bet(const bet_t bet) {
		bets.push_back(bet);
	}

	void print() const {
		cout << numbers;
	}
};