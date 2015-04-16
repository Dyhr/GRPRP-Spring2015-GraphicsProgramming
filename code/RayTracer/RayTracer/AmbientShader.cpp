#include "stdafx.h"
#include "AmbientShader.h"
#include "LightBase.h"

namespace RayTracer{
	AmbientShader::AmbientShader() : objectColor(ColorIntern(0, 0, 0, 0))
	{
	}

	AmbientShader::AmbientShader(ColorIntern objectColor) : objectColor(objectColor)
	{

	}

	ColorIntern AmbientShader::shade(Vector3d eyeVector, Vector3d normalToSurface, Vector3d pointOnObject, vector<LightBase*> lights)
	{
		ColorIntern colorToReturn = ColorIntern();

		for (vector<LightBase*>::iterator it = lights.begin(); it != lights.end(); ++it) {
			LightBase* light = *it;
			if ((light->getLightType()) == AMBIENT)
			{
				ColorIntern ambientColor = ColorIntern::intensifyColor(objectColor, light->GetIntensityOnPoint(pointOnObject));
				colorToReturn = ColorIntern::blendAddition(colorToReturn, ambientColor);
			}
		}
		return colorToReturn;
	}
}

