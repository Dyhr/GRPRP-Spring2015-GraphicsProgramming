#pragma once
#include <vector>

namespace RayTracer
{
	class ColorIntern
	{
		// Values for absorbance
		float redAbsorbance, greenAbsorbance, blueAbsorbance; // todo tror ikke vi skal bruge dem her - wind
	public:
		int red, green, blue, alpha;
		ColorIntern();
		ColorIntern(int red, int green, int blue, int alpha);

		static int sanitizeColor(int proposedValue);
		static ColorIntern blendAddition(ColorIntern colorA, ColorIntern colorB);
		static ColorIntern blendMultiply(ColorIntern colorA, ColorIntern colorB);
		static ColorIntern intensifyColor(ColorIntern colorA, float multiplier);
	};

	public ref class Color
	{
	public:
		int red, green, blue, alpha;
		Color();
		Color(int red, int green, int blue, int alpha);
		Color(ColorIntern color);
	};
}

