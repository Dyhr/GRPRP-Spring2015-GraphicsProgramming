#include "stdafx.h"
#include "Color.h"
#include <iostream>

namespace RayTracer
{
	Color::Color() : red(0), blue(0), green(0), alpha(255)
	{

	}

	Color::Color(ColorIntern color) : red(color.red), blue(color.blue), green(color.green), alpha(color.alpha)
	{
		red = red > 255 ? 255 : red;
		green = green > 255 ? 255 : green;
		blue = blue > 255 ? 255 : blue;

		red = red < 0 ? 0 : red;
		green = green < 0 ? 0 : green;
		blue = blue < 0 ? 0 : blue;
	}

	Color::Color(int red, int green, int blue, int alpha) : red(red), green(green), blue(blue), alpha(alpha)
	{
		if (red > 255 || green > 255 || blue > 255)
		{
			// throw exception
		}
		if (red < 0 || green < 0 || blue < 0)
		{
			// throw exception
		}
	}


	// Intern Color
	ColorIntern::ColorIntern() : red(0), blue(0), green(0), alpha(255)
	{

	}

	ColorIntern::ColorIntern(int red, int green, int blue, int alpha) : red(sanitizeColor(red)), green(sanitizeColor(green)), blue(sanitizeColor(blue)), alpha(alpha)
	{

	}

	ColorIntern ColorIntern::blendAddition(ColorIntern colorA, ColorIntern colorB)
	{
		int red = sanitizeColor(colorA.red + colorB.red);
		int green = sanitizeColor(colorA.green + colorB.green);
		int blue = sanitizeColor(colorA.blue + colorB.blue);
		return ColorIntern(red, green, blue, 255);
	}

	ColorIntern ColorIntern::intensifyColor(ColorIntern colorA, float multiplier)
	{
		int red = (int)sanitizeColor((int)(colorA.red * multiplier));
		int green = (int)sanitizeColor((int)(colorA.green * multiplier));
		int blue = (int)sanitizeColor((int)(colorA.blue * multiplier));

		return ColorIntern(red, green, blue, 255);
	}

	int ColorIntern::sanitizeColor(int proposedColorValue)
	{
		if (proposedColorValue > 255)
		{
			return 255;
		}
		else if (proposedColorValue < 0)
		{
			return 0;
		}

		return proposedColorValue;
	}
}

