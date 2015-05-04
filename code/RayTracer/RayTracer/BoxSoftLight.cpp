#include "stdafx.h"
#include "BoxSoftLight.h"
#include "PositionalLight.h"

namespace RayTracer{


	BoxSoftLight::BoxSoftLight()
	{
		lights = vector<LightBase*>(0);
	}

	BoxSoftLight::BoxSoftLight(float totalIntensity, Point3d position, float fallOff, ColorIntern color, float deltaX, float deltaY, float deltaZ, int amtOfLightsX, int amtOfLightsY, int amtOfLightsZ)
	{
		int lightsTimes3 = amtOfLightsX * amtOfLightsY * amtOfLightsZ;
		lights = vector<LightBase*>(lightsTimes3);
		int index = 0;
		for (size_t i = 0; i < amtOfLightsZ; i++)
		{
			for (size_t j = 0; j < amtOfLightsY; j++)
			{
				for (size_t k = 0; k < amtOfLightsX; k++)
				{
					Point3d point = Point3d(position.x - deltaX / 2.0f + deltaX / (float)(1.0f * amtOfLightsX) * (float)k,
											position.y - deltaY / 2.0f + deltaY / (float)(1.0f * amtOfLightsY) * (float)j,
											position.z - deltaZ / 2.0f + deltaZ / (float)(1.0f * amtOfLightsZ) * (float)i);
					lights[index++] = new PositionalLight(totalIntensity / lightsTimes3, point, fallOff, color);
				}
			}
		}
	}

	vector<LightBase*> BoxSoftLight::getLights()
	{
		return lights;
	}
}
