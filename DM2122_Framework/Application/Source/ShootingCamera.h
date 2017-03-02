#ifndef SHOOTINGCAMERA_H
#define SHOOTINGCAMERA_H

#include "Camera.h"
#include "AABB.h"
#include <vector>

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
	float velocity = 1;
	int X_Or_Z = 0;

	AABB charAABB;
	AABB objectAABB;
	int boundsCheck(Vector3);

	bool switchTreasure = false;
//===========Enemy sphere collision================
	std::vector <Vector3> enemyPos;
	bool collideEnemies(Vector3, Vector3);
	bool collideEnemy = false;
//=============Enemy hit notification==============
	void hitNoti(Vector3);
	int sideNoti[4];
	bool multipleEnemies = false;
//=================================================
	float mouseY = 0;
	float mouseX = 0;
	double xpos, ypos;
	double verticalAngle = 0.0f;
	double horizontalAngle = 0.0f;
	double rotateVert = 0;
	double rotateHori = 0;

//private:
	Vector3 view;
	Vector3 right;
};

#endif