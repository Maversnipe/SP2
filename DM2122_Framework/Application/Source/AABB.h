#ifndef AABB_H
#define AABB_H

struct Vec2D
{
	double X;
	double Z;
};

struct AABB
{
	void SaveCoord(Vector3 minPoint, Vector3 maxPoint)
	{
		min = minPoint;
		max = maxPoint;
	}

	void SaveBoundaryPoints(Vector3 minInput, Vector3 maxPoint)
	{
		min = minInput;
		max = maxPoint;
	}

	Vector3 min, max;
};
#endif