#ifndef CAMERA_3_H
#define CAMERA_3_H

#include "Camera.h"
#include "AABB.h"

class Camera3 : public Camera
{
public:
	//Vector3 position;
	//Vector3 target;
	//Vector3 up;

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	Camera3();
	~Camera3();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);

	bool buildingCollide(Vector3, Vector3);

	Vector3 buildings[5] = {};
	bool collideBuilding = false;
	Vector3 buildingSlide;
	int slideAABBbuilding(AABB, AABB);
	bool collideAABBbuilding(AABB, AABB);
	int buildingNum = 0;
	bool playCarousell = false;
	
	float mouseY = 0;
	float mouseX = 0;
	double xpos, ypos;
	float verticalAngle = 0.0f;
	float rotateVert = 0;
	float rotateHori = 0;
	double velocity = 1;
	int X_Or_Z = 0;

	AABB charAABB;
	AABB buildingAABB[5];


private:
	Vector3 view;
	Vector3 right;
};

#endif