#pragma once

#ifndef __MACRO_H
#define __MACRO_H

#include <math.h>

#define PI 3.1415926535
#define DegreeToRadian(X) X * PI / 180.

template<typename T>
T ClampAxis(T Angle)
{
	//[-360.0, 360.0]
	Angle = fmod(Angle, (T)360.0);

	if (Angle < (T)0.0)
	{
		Angle += (T)360.0;
	}

	return Angle;
}

template<typename T>
T NormalizeAxis(T Angle)
{
	// returns Angle in the range [0,360)
	Angle = ClampAxis(Angle);

	if (Angle > (T)180.0)
	{
		// shift to (-180,180]
		Angle -= (T)360.0;
	}

	return Angle;
}

#endif // !__MACRO_H

