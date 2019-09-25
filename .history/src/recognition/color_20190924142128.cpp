#include "color.h"

int HSV::toGetPixelPuyoColor()
{
	// HSV *hsv = new HSV();
	// toHSV(rgb, hsv);
	if (v < 100)
		return color::NONE;

	if (s < 50 && 120 < h)
		return color::DIST;

	if ((h <= 15 || 340 < h) /*&& 150 < v*/)
		return color::RED;

	if (20 <= h && h <= 60)
		return color::YELLOW;

	if (70 <= h && h <= 140)
		return color::GREEN;

	if ((160 <= h && h <= 255))
		return color::BLUE;

	if (250 <= h && (h < 340 && 150 < v))
		return color::PURPLE;

	return color::NONE;
}

int HSV::toReadBlueRed()
{
	if ((s < 40))
		return color::WHITE;

	if ((165 < h && h <= 270) && v > 60)
		return color::BLUE;

	if (((320 < h && h <= 360) || (0 <= h && h <= 30)) && v > 60)
		return color::RED;
	return color::OTHER;
}

int HSV::toReadGreenYellow()
{
	if ((50 < h) && (h < 70) && v > 200)
		return color::YELLOW;

	if ((90 < h) && (h < 120) && v > 100)
		return color::GREEN;

	return color::OTHER;
}
