#ifndef CAMERA_4_H
#define CAMERA_4_H

#include "Camera.h"
#include <vector>

class Camera4 : public Camera
{
public:
	//Vector3 position;
	//Vector3 target;
	//Vector3 up;

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;
	Vector3 view;
	Vector3 right;
	Vector3 carView;
	std::vector <Vector3> ObjPos;
	//Car car;

	Camera4();
	~Camera4();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt, float*,int&);
	virtual void Reset();

	int boundsCheck(Vector3);
	bool collision = false;
	float mouseY = 0;
	float mouseX = 0;
	double xpos, ypos;
	float verticalAngle = 0.0f;
	float horizontalAngle = 0.0f;
	float rotateVert = 0;
	float rotateHori = 0;
	float CAR_SPEED = 0.f;

private:

};

#endif