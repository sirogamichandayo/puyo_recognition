#include "color.h"

int HSV::toGetPixelPuyoColor()
{
	// HSV *hsv = new HSV();
	// toHSV(rgb, hsv);
	if (v < 150)
		return color::NONE;

	if (s < 50 && 120 < h)
		return color::DIST;

	if (h <= 15 && 70 < v)
		return color::RED;

	if (35 <= h && (h <= 70 && 150 < v))
		return color::YELLOW;

	if (85 <= h && (h <= 135 && 120 < v))
		return color::GREEN;

	if (160 <= h && (h <= 255 && 150 < v))
		return color::BLUE;

	if (250 <= h && (h < 340 && 150 < v))
		return color::PURPLE;

	return color::NONE;
}