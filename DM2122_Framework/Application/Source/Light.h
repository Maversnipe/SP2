#ifndef LIGHT_H_
#define LIGHT_H_
#include "Vertex.h"

struct Light
{
	Position position;
	Color color;
	float power = 0;
	float kC = 0, kL = 0, kQ = 0;
	Light(){}

	enum LIGHT_TYPE
	{
		LIGHT_POINT = 0,
		LIGHT_DIRECTIONAL,
		LIGHT_SPOT,
	};
	LIGHT_TYPE type;
	Vector3 spotDirection;
	float cosCutoff;
	float cosInner;
	float exponent;
	
};



#endif