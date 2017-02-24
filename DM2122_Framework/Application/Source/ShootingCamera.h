#ifndef SHOOTINGCAMERA_H
#define SHOOTINGCAMERA_H

#include "Camera.h"
#include "AABB.h"

class ShootingCamera : public Camera
{
public:

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	ShootingCamera();
	~ShootingCamera();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt, float* horizontal, float* vertical);
//==============AABB colliding====================

	Vector3 object;
	bool collideBuilding = false;
	Vector3 buildingSlide;
	int slideAABBobject(AABB, AABB);
	bool collideAABBobject(AABB, AABB);
	double velocity = 1;
	int X_Or_Z = 0;

	AABB charAABB;
	AABB objectAABB;

	bool switchTreasure = false;
//=================================================
	float mouseY = 0;
	float mouseX = 0;
	double xpos, ypos;
	float verticalAngle = 0.0f;
	float horizontalAngle = 0.0f;
	float rotateVert = 0;
	float rotateHori = 0;

//private:
	Vector3 view;
	Vector3 right;
};

#endif