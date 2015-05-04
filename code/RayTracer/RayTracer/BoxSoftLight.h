#pragma once
#include "SoftLightbase.h"
namespace RayTracer{
	class BoxSoftLight :
		public SoftLightbase
	{
		vector<LightBase*> lights;
	public:
		BoxSoftLight();
		/// Creates an distribution which creates equal amount of lights on each of the axis.
		BoxSoftLight(float totalIntensity, Point3d position, float fallOff, ColorIntern color, float deltaX, float deltaY, float deltaZ, int amtOfLights);

		vector<LightBase*> getLights();
	};
}
