#pragma once

#ifndef __MATERIAL_H
#define __MATERIAL_H

#include <string>

struct Material
{
	std::string diffuse;
	std::string specular;
	bool bEmission = false;
	std::string emission;
	int shininess = 32;
};
#endif // !__MATERIAL_H


