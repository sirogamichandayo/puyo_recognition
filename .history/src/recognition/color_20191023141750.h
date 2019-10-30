#ifndef COLOR_H
#define COLOR_H

#include <iostream>

namespace color
{
	unsigned const int PUYO_COLOR_NUM = 4;

	unsigned const int NONE = 0;
	unsigned const int DIST = 1; 		// Disturb
	unsigned const int WALL = 2;
	unsigned const int IRON = 3;
	unsigned const int RED  = 4;
	unsigned const int BLUE = 5;
	unsigned const int YELLOW = 6;
	unsigned const int GREEN = 7;
	unsigned const int PURPLE = 8;			// purple
	unsigned const int WHITE = 9;
	unsigned const int ORANGE = 10;    // judge clear or not.(for TA)
	unsigned const int OTHER = 100000;
	unsigned const int MISS = 9999999;
	}; // namespace tumo

struct HSV
{
	int h, s, v;
	bool init;

	HSV(const int h_, const int s_, const int v_)
		: h(h_), s(s_), v(v_), init(true) {}
	HSV() {};

	void show_all() {
		std::cout << "(h:" << h << ", s: " << s << ", v:" << v << ")" << std::endl;
	}

	// assignment hsv value
	void operator() (const int h_, const int s_, const int v_)
	{
		h = h_; s = s_;	v = v_;
	}

	int toGetPixelPuyoColor();
	int toReadBlueRed();
	int toReadGreenYellow();
	int toReadOrange();
};

#endif // COLOR_H