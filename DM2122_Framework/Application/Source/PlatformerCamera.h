#ifndef PLATFORMER_CAMERA_H
#define PLATFORMER_CAMERA_H

#include "Camera.h"
#include "AABB.h"

class PlatformerCamera : public Camera
{
public:

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	PlatformerCamera();
	~PlatformerCamera();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);

	float mouseY = 0;
	float mouseX = 0;
	double xpos, ypos;
	float verticalAngle = 0.0f;
	float rotateVert = 0;
	float rotateHori = 0;
	double velocity = 1;


private:
	Vector3 view;
	Vector3 right;
};

#endif