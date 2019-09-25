#include <iostream>
using namespace std;

struct RGB
{
	int r, g, b;
	void show_all() {
		cout << "(r:" << r << ", g: " << g << ", b:" << b << ")" << endl;
	}
};

struct HSV
{
	int h, s, v;

	void show_all() {
		cout << "(h:" << h << ", s: " << s << ", v:" << v << ")" << endl;
	}

	// assignment hsv value
	void operator() (int h_, int s_, int v_)
	{
		h = h_; s = s_;	v = v_;
	}
};

/*
namespace state
{
	unsigned const int
	unsigned const int
	unsigned const int
	unsigned const int
	unsigned const int
	unsigned const int
}
*/

namespace color
{
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
	unsigned const int OTHER = 10;
	}; // namespace tumo
