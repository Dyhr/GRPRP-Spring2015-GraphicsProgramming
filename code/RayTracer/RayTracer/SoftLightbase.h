#pragma once
#include "LightBase.h"
#include "Vector3d.h"
#include "Point3d.h"
#include <vector> 

class SoftLightbase
{
public:
	virtual vector<LightBase*> getLightsOnPoint(Point3d point) = 0;
};

