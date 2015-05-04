#pragma once
#include "SoftLightbase.h"
namespace RayTracer{
	class BoxSoftLight :
		public SoftLightbase
	{
		vector<LightBase*> lights;
	public:
		BoxSoftLight();
		BoxSoftLight(float TotalIntensity, Point3d position, float fallOff, ColorIntern color, float deltaX, float deltaY, float deltaZ);

		vector<LightBase*> getLights();
	};
}
