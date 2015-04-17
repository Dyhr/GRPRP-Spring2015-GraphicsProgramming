#include "stdafx.h"
#include "DiffuseShader.h"

namespace RayTracer{
	DiffuseShader::DiffuseShader()
	{
	}

	DiffuseShader::DiffuseShader(ColorIntern objectColor) : objectColor(objectColor)
	{

	}

	ColorIntern DiffuseShader::shade(Vector3d eyeVector, Vector3d normalToSurface, Vector3d pointOnObject, vector<LightBase*> lights)
	{
		ColorIntern colorToReturn = ColorIntern();

		for (vector<LightBase*>::iterator it = lights.begin(); it != lights.end(); ++it) {
			LightBase* light = *it;
			if ((light->getLightType()) == AMBIENT)
			{
				ColorIntern ambientColor = ColorIntern::intensifyColor(objectColor, light->GetIntensityOnPoint(pointOnObject));
				colorToReturn = ColorIntern::blendAddition(colorToReturn, ambientColor);
			}
			else
			{
				Vector3d normalNormalized = Vector3d::normalize(normalToSurface);
				Vector3d lightIncommingNormalized = Vector3d::normalize(light->GetLightOnPoint(pointOnObject));

				float intensity = (Vector3d::dotProduct(normalNormalized, lightIncommingNormalized))*light->GetIntensityOnPoint(pointOnObject);

				ColorIntern diffuseColor = ColorIntern::intensifyColor(objectColor, intensity);
				colorToReturn = ColorIntern::blendAddition(colorToReturn, diffuseColor);
			}
		}
		return colorToReturn;
	}
}
