#include "Scene.h"

namespace RayTracer
{
	class ShaderBase
	{
	public:
		ColorIntern shade(Vector3d eyeVector, Vector3d normalToSurface, Vector3d pointOnObject, vector<LightBase*> lights);
	};
}