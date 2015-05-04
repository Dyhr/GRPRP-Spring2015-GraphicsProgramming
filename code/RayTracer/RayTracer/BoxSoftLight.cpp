#include "stdafx.h"
#include "BoxSoftLight.h"
#include "PositionalLight.h"

namespace RayTracer{


	BoxSoftLight::BoxSoftLight()
	{
		lights = vector<LightBase*>(0);
	}

	BoxSoftLight::BoxSoftLight(float totalIntensity, Point3d position, float fallOff, ColorIntern color, float deltaX, float deltaY, float deltaZ, int amtOfLights)
	{
		int lightsTimes3 = amtOfLights * amtOfLights * amtOfLights;
		lights = vector<LightBase*>(lightsTimes3);
		int index = 0;
		for (size_t i = 0; i < amtOfLights; i++)
		{
			for (size_t j = 0; j < amtOfLights; j++)
			{
				for (size_t k = 0; k < amtOfLights; k++)
				{
					Point3d point = Point3d(position.x - deltaX / 2 + deltaX / amtOfLights * k,
											position.y - deltaY / 2 + deltaY / amtOfLights * j,
											position.z - deltaZ / 2 + deltaZ / amtOfLights * i);
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
