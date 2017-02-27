#ifndef PLATFORMER_CAMERA_H
#define PLATFORMER_CAMERA_H

#include "Camera.h"
#include "AABB.h"
#include <vector>
#include "Platforms.h"

class PlatformerCamera : public Camera
{
public:

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	PlatformerCamera();
	~PlatformerCamera();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt, std::vector<Platforms> []);

	float mouseY = 0;
	float mouseX = 0;
	double xpos, ypos;
	float verticalAngle = 0.0f;
	float rotateVert = 0;
	float rotateHori = 0;
	double velocity = 1;
	double fallingVelocity = 20;

	// Platform Stuff
	bool onPlatform(AABB, AABB);
	AABB charAABB;
	Platforms currPlatform;
	Platforms nextPlatform;
	Platforms noPlatform;

	// Jumping
	bool jump = false;
	float playerOriginalHeight = 0.0;

private:
	Vector3 view;
	Vector3 right;
};

#endif