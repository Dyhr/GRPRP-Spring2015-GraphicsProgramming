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
		int red =	sanitizeColor(colorA.red + colorB.red);
		int green = sanitizeColor(colorA.green + colorB.green);
		int blue =	sanitizeColor(colorA.blue + colorB.blue);
		return ColorIntern(red, green, blue, 255);
	}

	ColorIntern ColorIntern::blendMultiply(ColorIntern colorA, ColorIntern colorB)
	{
		int red =	sanitizeColor(colorA.red * colorB.red / 255);
		int green = sanitizeColor(colorA.green * colorB.green / 255);
		int blue =	sanitizeColor(colorA.blue * colorB.blue / 255);
		return ColorIntern(red, green, blue, 255);
	}

	ColorIntern ColorIntern::intensifyColor(ColorIntern colorA, float multiplier)
	{
		int red =	sanitizeColor(colorA.red * multiplier);
		int green = sanitizeColor(colorA.green * multiplier);
		int blue =	sanitizeColor(colorA.blue * multiplier);

		return ColorIntern(red, green, blue, 255);
	}

	int ColorIntern::sanitizeColor(int proposedColorValue)
	{
		if (proposedColorValue > 255)
		{
			return (int)255;
		}
		else if (proposedColorValue < 0)
		{
			return (int)0;
		}

		return (int)proposedColorValue;
	}

	// ratioAtoB must be a number between 0 and 1 
	ColorIntern ColorIntern::blendByAmount(ColorIntern colorA, ColorIntern colorB, float ratioAToB)
	{
		if (ratioAToB < 0.0 || ratioAToB > 1.0)
		{
			// TODO: Throw relevant exception
		}

		int newRed = sanitizeColor(colorA.red * ratioAToB + colorB.red *(1.0 - ratioAToB));
		int newGreen = sanitizeColor(colorA.green * ratioAToB + colorB.green *(1.0 - ratioAToB));
		int newBlue = sanitizeColor(colorA.blue * ratioAToB + colorB.blue *(1.0 - ratioAToB));

		return ColorIntern(newRed, newGreen, newBlue, 255);
	}


}

