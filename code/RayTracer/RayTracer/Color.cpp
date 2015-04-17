#include "stdafx.h"
#include "Color.h"
#include <sstream>

namespace RayTracer
{
	Color::Color() : red(0), blue(0), green(0), alpha(255)
	{

	}

	Color::Color(ColorIntern color) : red(color.red), blue(color.blue), green(color.green), alpha(color.alpha)
	{

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

	ColorIntern::ColorIntern(int red, int green, int blue, int alpha) : red(red), green(green), blue(blue), alpha(alpha)
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

	ColorIntern ColorIntern::blendAddition(ColorIntern colorA, ColorIntern colorB)
	{
		int red = ((colorA.red + colorB.red)		> 255 ? 255 : (colorA.red + colorB.red))	< 0 ? 0 : (colorA.red + colorB.red);
		int green = (colorA.green + colorB.green)	> 255 ? 255 : (colorA.green + colorB.green) < 0 ? 0 : (colorA.green + colorB.green);
		int blue = (colorA.blue + colorB.blue)		> 255 ? 255 : (colorA.blue + colorB.blue)	< 0 ? 0 : (colorA.blue + colorB.blue);
		
		return ColorIntern(red, green, blue, 255);
	}

	ColorIntern ColorIntern::intensifyColor(ColorIntern colorA, float multiplier)
	{
		int red = ((colorA.red*multiplier)		> 254 ? 255 : (colorA.red*multiplier))		< 0 ? 0 : (colorA.red*multiplier);
		int green = ((colorA.green*multiplier)	> 254 ? 255 : (colorA.green*multiplier))	< 0 ? 0 : (colorA.green*multiplier);
		int blue = ((colorA.blue*multiplier)	> 254 ? 255 : (colorA.blue*multiplier))		< 0 ? 0 : (colorA.blue*multiplier);
		return ColorIntern(red, green, blue, 255);
	}

}

