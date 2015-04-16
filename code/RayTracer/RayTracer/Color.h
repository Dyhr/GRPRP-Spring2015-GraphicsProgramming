#pragma once
namespace RayTracer
{
	public ref class Color
	{
	public:
		int red, green, blue, alpha;
		Color();
		Color(int red, int green, int blue, int alpha);
	};

	class ColorIntern
	{
	public:
		int red, green, blue, alpha;
		ColorIntern();
		ColorIntern(int red, int green, int blue, int alpha);

		static ColorIntern blendAddition(ColorIntern colorA, ColorIntern colorB);
		static ColorIntern intensifyColor(ColorIntern colorA, float multiplier);
	};
}

